/* ============================================================================
* Copyright (c) 2009-2018 BlueQuartz Software, LLC
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright notice, this
* list of conditions and the following disclaimer in the documentation and/or
* other materials provided with the distribution.
*
* Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
* contributors may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* The code contained herein was partially funded by the followig contracts:
*    United States Air Force Prime Contract FA8650-07-D-5800
*    United States Air Force Prime Contract FA8650-10-D-5210
*    United States Prime Contract Navy N00173-07-C-2068
*    United States Air Force Prime Contract FA8650-18-C-5270
*
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "FindGroupingDensity.h"

#include "SIMPLib/Common/Constants.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/AttributeMatrixSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/DataArraySelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Math/SIMPLibMath.h"

#include "Statistics/StatisticsConstants.h"
#include "Statistics/StatisticsVersion.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FindGroupingDensity::FindGroupingDensity()
  : AbstractFilter()
  , m_FeatureAttributeMatrixName("", "", "")
  , m_ParentAttributeMatrixName("", "", "")
  , m_ContiguousNeighborListArrayPath("", "", "")
  , m_NonContiguousNeighborListArrayPath("", "", "")
  , m_ParentIdsArrayPath("", "", SIMPL::FeatureData::ParentIds)
  , m_VolumesArrayPath("", "", SIMPL::FeatureData::Volumes)
  , m_ParentVolumesArrayPath("", "", SIMPL::FeatureData::ParentVolumes)
  , m_ParentDensitiesArrayName("GroupingDensities")
  , m_CheckedFeaturesArrayName("CheckedFeatures")
  , m_ParentIds(nullptr)
  , m_Volumes(nullptr)
  , m_ParentVolumes(nullptr)
  , m_ParentDensities(nullptr)
  , m_CheckedFeatures(nullptr)
  , m_UseNonContiguousNeighbors(false)
  , m_FindCheckedFeatures(false)
{
  m_ContiguousNeighborList = NeighborList<int32_t>::NullPointer();
  m_NonContiguousNeighborList = NeighborList<int32_t>::NullPointer();

  setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FindGroupingDensity::~FindGroupingDensity() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindGroupingDensity::setupFilterParameters()
{
  FilterParameterVector parameters;
  parameters.push_back(SeparatorFilterParameter::New("Feature Data", FilterParameter::RequiredArray));
  {
    AttributeMatrixSelectionFilterParameter::RequirementType req =
        AttributeMatrixSelectionFilterParameter::CreateRequirement(AttributeMatrix::Type::Any, IGeometry::Type::Any);
     AttributeMatrix::Types amTypes = { AttributeMatrix::Type::VertexFeature, AttributeMatrix::Type::EdgeFeature,
                                  AttributeMatrix::Type::FaceFeature, AttributeMatrix::Type::CellFeature };
    req.amTypes = amTypes;
    parameters.push_back(SIMPL_NEW_AM_SELECTION_FP("Feature Attribute Matrix", FeatureAttributeMatrixName, FilterParameter::RequiredArray, FindGroupingDensity, req));
  }
  {
	  DataArraySelectionFilterParameter::RequirementType req =
		  DataArraySelectionFilterParameter::CreateRequirement(SIMPL::TypeNames::Float, 1, AttributeMatrix::Type::Any, IGeometry::Type::Any);
	  AttributeMatrix::Types amTypes = { AttributeMatrix::Type::Vertex, AttributeMatrix::Type::Edge,
		  AttributeMatrix::Type::Face, AttributeMatrix::Type::CellFeature };
	  req.amTypes = amTypes;
	  parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Volumes", VolumesArrayPath, FilterParameter::RequiredArray, FindGroupingDensity, req));
  }
  {
	  DataArraySelectionFilterParameter::RequirementType req = DataArraySelectionFilterParameter::CreateCategoryRequirement(SIMPL::TypeNames::NeighborList, 1, AttributeMatrix::Category::Feature);
	  parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Contiguous Neighbor List", ContiguousNeighborListArrayPath, FilterParameter::RequiredArray, FindGroupingDensity, req));
  }
  QStringList linkedProps("NonContiguousNeighborListArrayPath");
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Use Non-Contiguous Neighbors", UseNonContiguousNeighbors, FilterParameter::Parameter, FindGroupingDensity, linkedProps));
  {
	  DataArraySelectionFilterParameter::RequirementType req = DataArraySelectionFilterParameter::CreateCategoryRequirement(SIMPL::TypeNames::NeighborList, 1, AttributeMatrix::Category::Feature);
	  parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Non-Contiguous Neighbor List", NonContiguousNeighborListArrayPath, FilterParameter::RequiredArray, FindGroupingDensity, req));
  }
  {
	  DataArraySelectionFilterParameter::RequirementType req =
		  DataArraySelectionFilterParameter::CreateRequirement(SIMPL::TypeNames::Int32, 1, AttributeMatrix::Type::Any, IGeometry::Type::Any);
	  AttributeMatrix::Types amTypes = { AttributeMatrix::Type::Vertex, AttributeMatrix::Type::Edge,
		  AttributeMatrix::Type::Face, AttributeMatrix::Type::CellFeature };
	  req.amTypes = amTypes;
	  parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Parent Ids", ParentIdsArrayPath, FilterParameter::RequiredArray, FindGroupingDensity, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Parent Data", FilterParameter::RequiredArray));
  {
	  AttributeMatrixSelectionFilterParameter::RequirementType req =
		  AttributeMatrixSelectionFilterParameter::CreateRequirement(AttributeMatrix::Type::Any, IGeometry::Type::Any);
	  AttributeMatrix::Types amTypes = { AttributeMatrix::Type::VertexFeature, AttributeMatrix::Type::EdgeFeature,
		  AttributeMatrix::Type::FaceFeature, AttributeMatrix::Type::CellFeature };
	  req.amTypes = amTypes;
	  parameters.push_back(SIMPL_NEW_AM_SELECTION_FP("Parent Attribute Matrix", ParentAttributeMatrixName, FilterParameter::RequiredArray, FindGroupingDensity, req));
  }
  {
	  DataArraySelectionFilterParameter::RequirementType req =
		  DataArraySelectionFilterParameter::CreateRequirement(SIMPL::TypeNames::Float, 1, AttributeMatrix::Type::Any, IGeometry::Type::Any);
	  AttributeMatrix::Types amTypes = { AttributeMatrix::Type::Vertex, AttributeMatrix::Type::Edge,
		  AttributeMatrix::Type::Face, AttributeMatrix::Type::CellFeature };
	  req.amTypes = amTypes;
	  parameters.push_back(SIMPL_NEW_DA_SELECTION_FP("Parent Volumes", ParentVolumesArrayPath, FilterParameter::RequiredArray, FindGroupingDensity, req));
  }
  parameters.push_back(SeparatorFilterParameter::New("Feature Data", FilterParameter::CreatedArray));
  QStringList linkedProps2("CheckedFeaturesArrayName");
  parameters.push_back(SIMPL_NEW_LINKED_BOOL_FP("Find Checked Features", FindCheckedFeatures, FilterParameter::Parameter, FindGroupingDensity, linkedProps2));
  parameters.push_back(SIMPL_NEW_STRING_FP("Checked Features", CheckedFeaturesArrayName, FilterParameter::CreatedArray, FindGroupingDensity));
  parameters.push_back(SeparatorFilterParameter::New("Parent Data", FilterParameter::CreatedArray));
  parameters.push_back(SIMPL_NEW_STRING_FP("Grouping Densities", ParentDensitiesArrayName, FilterParameter::CreatedArray, FindGroupingDensity));
  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindGroupingDensity::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
  reader->openFilterGroup(this, index);
  setFeatureAttributeMatrixName(reader->readDataArrayPath("CellFeatureAttributeMatrixName", getFeatureAttributeMatrixName())); // Note the String name differs, this is for historical reasons
  setParentAttributeMatrixName(reader->readDataArrayPath("CellFeatureAttributeMatrixName", getParentAttributeMatrixName())); // Note the String name differs, this is for historical reasons
  setParentIdsArrayPath(reader->readDataArrayPath("ParentIdsArrayPath", getParentIdsArrayPath()));
  setVolumesArrayPath(reader->readDataArrayPath("VolumesArrayPath", getVolumesArrayPath()));
  setParentVolumesArrayPath(reader->readDataArrayPath("ParentVolumesArrayPath", getParentVolumesArrayPath()));
  setParentDensitiesArrayName(reader->readString("ParentDensitiesArrayName", getParentDensitiesArrayName()));
  setCheckedFeaturesArrayName(reader->readString("CheckedFeaturesArrayName", getCheckedFeaturesArrayName()));
  setUseNonContiguousNeighbors(reader->readValue("UseNonContiguousNeighbors", getUseNonContiguousNeighbors()));
  setFindCheckedFeatures(reader->readValue("FindCheckedFeatures", getFindCheckedFeatures()));
  setContiguousNeighborListArrayPath(reader->readDataArrayPath("ContiguousNeighborListArrayPath", getContiguousNeighborListArrayPath()));
  setNonContiguousNeighborListArrayPath(reader->readDataArrayPath("NonContiguousNeighborListArrayPath", getNonContiguousNeighborListArrayPath()));
  reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindGroupingDensity::initialize()
{
  m_ContiguousNeighborList = NeighborList<int32_t>::NullPointer();
  m_NonContiguousNeighborList = NeighborList<int32_t>::NullPointer();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindGroupingDensity::dataCheck()
{
  setErrorCondition(0);
  setWarningCondition(0);
  DataArrayPath tempPath;

  QVector<IDataArray::Pointer> dataArrays;

  IGeometry::Pointer igeom = getDataContainerArray()->getPrereqGeometryFromDataContainer<IGeometry, AbstractFilter>(this, getParentIdsArrayPath().getDataContainerName());

  QVector<size_t> cDims(1, 1);
  m_ParentIdsPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<int32_t>, AbstractFilter>(this, getParentIdsArrayPath(),
                                                                                                        cDims);
  if(nullptr != m_ParentIdsPtr.lock().get())
  {
    m_ParentIds = m_ParentIdsPtr.lock()->getPointer(0);
  }

  m_ParentVolumesPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<float>, AbstractFilter>(this, getParentVolumesArrayPath(),
	  cDims);
  if (nullptr != m_ParentVolumesPtr.lock().get())
  {
	  m_ParentVolumes = m_ParentVolumesPtr.lock()->getPointer(0);
  }

  tempPath.update(getParentAttributeMatrixName().getDataContainerName(), getParentAttributeMatrixName().getAttributeMatrixName(), getParentDensitiesArrayName());
  m_ParentDensitiesPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<float>, AbstractFilter, float>(this, tempPath, 0,
	  cDims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
  if (nullptr != m_ParentDensitiesPtr.lock().get()) /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
  {
	  m_ParentDensities = m_ParentDensitiesPtr.lock()->getPointer(0);
  }

  if (m_FindCheckedFeatures == true)
  {
	  tempPath.update(getFeatureAttributeMatrixName().getDataContainerName(), getFeatureAttributeMatrixName().getAttributeMatrixName(), getCheckedFeaturesArrayName());
	  m_CheckedFeaturesPtr = getDataContainerArray()->createNonPrereqArrayFromPath<DataArray<int32_t>, AbstractFilter, int32_t>(this, tempPath, 0,
		  cDims); /* Assigns the shared_ptr<> to an instance variable that is a weak_ptr<> */
	  if (nullptr != m_CheckedFeaturesPtr.lock().get()) /* Validate the Weak Pointer wraps a non-nullptr pointer to a DataArray<T> object */
	  {
		  m_CheckedFeatures = m_CheckedFeaturesPtr.lock()->getPointer(0);
	  } /* Now assign the raw pointer to data from the DataArray<T> object */
  }

  m_VolumesPtr = getDataContainerArray()->getPrereqArrayFromPath<DataArray<float>, AbstractFilter>(this, getVolumesArrayPath(),
	  cDims);
  if (nullptr != m_VolumesPtr.lock().get())
  {
	  m_Volumes = m_VolumesPtr.lock()->getPointer(0);
  }

  initialize();

  m_ContiguousNeighborList = getDataContainerArray()->getPrereqArrayFromPath<NeighborList<int32_t>, AbstractFilter>(this, getContiguousNeighborListArrayPath(), cDims);
  if (m_UseNonContiguousNeighbors == true)
  {
	  m_NonContiguousNeighborList = getDataContainerArray()->getPrereqArrayFromPath<NeighborList<int32_t>, AbstractFilter>(this, getNonContiguousNeighborListArrayPath(), cDims);
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindGroupingDensity::preflight()
{
  setInPreflight(true);
  emit preflightAboutToExecute();
  emit updateFilterParameters(this);
  dataCheck();
  emit preflightExecuted();
  setInPreflight(false);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindGroupingDensity::execute()
{
  setErrorCondition(0);
  setWarningCondition(0);
  dataCheck();
  if(getErrorCondition() < 0)
  {
    return;
  }

  DataContainer::Pointer m = getDataContainerArray()->getDataContainer(m_VolumesArrayPath.getDataContainerName());

  size_t numfeatures = m_VolumesPtr.lock()->getNumberOfTuples();
  size_t numparents = m_ParentVolumesPtr.lock()->getNumberOfTuples();

  NeighborList<int32_t>* neighborlist = m_ContiguousNeighborList.lock().get();
  NeighborList<int32_t>& neighborhoodlist = *(m_NonContiguousNeighborList.lock());

  int kmax = 1;
  if (m_UseNonContiguousNeighbors == true) { kmax = 2; }

  int32_t numneighbors = 0, numneighborhoods = 0, numcurneighborlist = 0, neigh = 0;
  float totalCheckVolume = 0.0f, curParentVolume = 0.0f, curParentDensities = 0.0f;
  QVector<int32_t> totalCheckList;
  QVector<float> checkedfeaturevolumes(numfeatures, 0.0f);

  for (size_t i = 1; i < numparents; i++)
  {
	  for (size_t j = 1; j < numfeatures; j++)
	  {
		  if (m_ParentIds[j] == i)
		  {
			  if (totalCheckList.contains(j) == false)
			  {
				  totalCheckVolume += m_Volumes[j];
				  totalCheckList.push_back(j);
				  if (m_ParentVolumes[i] > checkedfeaturevolumes[j] && m_FindCheckedFeatures == true)
				  {
					  checkedfeaturevolumes[j] = m_ParentVolumes[i];
					  m_CheckedFeatures[j] = i;
				  }
			  }
			  numneighbors = neighborlist->getListSize(j);
			  if (m_UseNonContiguousNeighbors == true)
			  {
				  numneighborhoods = int32_t(neighborhoodlist[j].size());
			  }
			  for (int k = 0; k < kmax; k++)
			  {
				  if (k == 0) { numcurneighborlist = numneighbors; }
				  else if (k == 1) { numcurneighborlist = numneighborhoods; }
				  for (int32_t l = 0; l < numcurneighborlist; l++)
				  {
					  if (k == 0) { neigh = neighborlist->getListReference(j)[l]; }
					  else if (k == 1) { neigh = neighborhoodlist[j][l]; }
					  if (totalCheckList.contains(neigh) == false)
					  {
						  totalCheckVolume += m_Volumes[neigh];
						  totalCheckList.push_back(neigh);
						  if (m_ParentVolumes[i] > checkedfeaturevolumes[neigh] && m_FindCheckedFeatures == true)
						  {
							  checkedfeaturevolumes[neigh] = m_ParentVolumes[i];
							  m_CheckedFeatures[neigh] = i;
						  }
					  }
				  }
			  }
		  }
	  }
	  curParentVolume = m_ParentVolumes[i];
	  if (totalCheckVolume == 0.0f) { m_ParentDensities[i] = -1.0f; }
	  else
	  {
		  curParentDensities = (curParentVolume / totalCheckVolume);
		  m_ParentDensities[i] = curParentDensities;
	  }
	  totalCheckList.resize(0);
	  totalCheckVolume = 0.0f;
  }

  notifyStatusMessage(getHumanLabel(), "Complete");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer FindGroupingDensity::newFilterInstance(bool copyFilterParameters) const
{
  FindGroupingDensity::Pointer filter = FindGroupingDensity::New();
  if(true == copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindGroupingDensity::getCompiledLibraryName() const
{
  return StatisticsConstants::StatisticsBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindGroupingDensity::getBrandingString() const
{
  return "Statistics";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindGroupingDensity::getFilterVersion() const
{
  QString version;
  QTextStream vStream(&version);
  vStream << Statistics::Version::Major() << "." << Statistics::Version::Minor() << "." << Statistics::Version::Patch();
  return version;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindGroupingDensity::getGroupName() const
{
  return SIMPL::FilterGroups::StatisticsFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid FindGroupingDensity::getUuid()
{
  return QUuid("{708be082-8b08-4db2-94be-52781ed4d53d}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindGroupingDensity::getSubGroupName() const
{
	return SIMPL::FilterGroups::ReconstructionFilters;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString FindGroupingDensity::getHumanLabel() const
{
  return "Find Grouping Densities";
}
