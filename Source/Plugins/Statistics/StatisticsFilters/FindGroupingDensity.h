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

#ifndef _findgroupingdensity_h_
#define _findgroupingdensity_h_

#include "SIMPLib/Common/SIMPLibSetGetMacros.h"
#include "SIMPLib/Filtering/AbstractFilter.h"
#include "SIMPLib/Geometry/ImageGeom.h"
#include "SIMPLib/SIMPLib.h"
#include "SIMPLib/DataArrays/NeighborList.hpp"

/**
 * @brief The FindGroupingDensity class. See [Filter documentation](@ref findgroupingdensity) for details.
 */
class FindGroupingDensity : public AbstractFilter
{
  Q_OBJECT
public:
  SIMPL_SHARED_POINTERS(FindGroupingDensity)
  SIMPL_STATIC_NEW_MACRO(FindGroupingDensity)
   SIMPL_TYPE_MACRO_SUPER_OVERRIDE(FindGroupingDensity, AbstractFilter)

  virtual ~FindGroupingDensity();

  SIMPL_FILTER_PARAMETER(DataArrayPath, FeatureAttributeMatrixName)
  Q_PROPERTY(DataArrayPath FeatureAttributeMatrixName READ getFeatureAttributeMatrixName WRITE setFeatureAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(DataArrayPath, ParentAttributeMatrixName)
  Q_PROPERTY(DataArrayPath ParentAttributeMatrixName READ getParentAttributeMatrixName WRITE setParentAttributeMatrixName)

  SIMPL_FILTER_PARAMETER(DataArrayPath, ParentIdsArrayPath)
  Q_PROPERTY(DataArrayPath ParentIdsArrayPath READ getParentIdsArrayPath WRITE setParentIdsArrayPath)
	  
  SIMPL_FILTER_PARAMETER(DataArrayPath, VolumesArrayPath)
  Q_PROPERTY(DataArrayPath VolumesArrayPath READ getVolumesArrayPath WRITE setVolumesArrayPath)

  SIMPL_FILTER_PARAMETER(DataArrayPath, ParentVolumesArrayPath)
  Q_PROPERTY(DataArrayPath ParentVolumesArrayPath READ getParentVolumesArrayPath WRITE setParentVolumesArrayPath)

  SIMPL_FILTER_PARAMETER(QString, ParentDensitiesArrayName)
  Q_PROPERTY(QString ParentDensitiesArrayName READ getParentDensitiesArrayName WRITE setParentDensitiesArrayName)

  SIMPL_FILTER_PARAMETER(QString, CheckedFeaturesArrayName)
  Q_PROPERTY(QString CheckedFeaturesArrayName READ getCheckedFeaturesArrayName WRITE setCheckedFeaturesArrayName)

  SIMPL_FILTER_PARAMETER(DataArrayPath, ContiguousNeighborListArrayPath)
  Q_PROPERTY(DataArrayPath ContiguousNeighborListArrayPath READ getContiguousNeighborListArrayPath WRITE setContiguousNeighborListArrayPath)

  SIMPL_FILTER_PARAMETER(DataArrayPath, NonContiguousNeighborListArrayPath)
  Q_PROPERTY(DataArrayPath NonContiguousNeighborListArrayPath READ getNonContiguousNeighborListArrayPath WRITE setNonContiguousNeighborListArrayPath)

  SIMPL_FILTER_PARAMETER(bool, UseNonContiguousNeighbors)
  Q_PROPERTY(float UseNonContiguousNeighbors READ getUseNonContiguousNeighbors WRITE setUseNonContiguousNeighbors)

  SIMPL_FILTER_PARAMETER(bool, FindCheckedFeatures)
  Q_PROPERTY(float FindCheckedFeatures READ getFindCheckedFeatures WRITE setFindCheckedFeatures)

  /**
   * @brief getCompiledLibraryName Reimplemented from @see AbstractFilter class
   */
  virtual const QString getCompiledLibraryName() const override;

  /**
   * @brief getBrandingString Returns the branding string for the filter, which is a tag
   * used to denote the filter's association with specific plugins
   * @return Branding string
  */
  virtual const QString getBrandingString() const override;

  /**
   * @brief getFilterVersion Returns a version string for this filter. Default
   * value is an empty string.
   * @return
   */
  virtual const QString getFilterVersion() const override;

  /**
   * @brief newFilterInstance Reimplemented from @see AbstractFilter class
   */
  virtual AbstractFilter::Pointer newFilterInstance(bool copyFilterParameters) const override;

  /**
   * @brief getGroupName Reimplemented from @see AbstractFilter class
   */
  virtual const QString getGroupName() const override;

  /**
   * @brief getSubGroupName Reimplemented from @see AbstractFilter class
   */
  virtual const QString getSubGroupName() const override;

  /**
   * @brief getUuid Return the unique identifier for this filter.
   * @return A QUuid object.
   */
  virtual const QUuid getUuid() override;

  /**
   * @brief getHumanLabel Reimplemented from @see AbstractFilter class
   */
  virtual const QString getHumanLabel() const override;

  /**
   * @brief setupFilterParameters Reimplemented from @see AbstractFilter class
   */
  virtual void setupFilterParameters() override;

  /**
   * @brief readFilterParameters Reimplemented from @see AbstractFilter class
   */
  virtual void readFilterParameters(AbstractFilterParametersReader* reader, int index);

  /**
   * @brief execute Reimplemented from @see AbstractFilter class
   */
  virtual void execute() override;

  /**
  * @brief preflight Reimplemented from @see AbstractFilter class
  */
  virtual void preflight() override;

signals:
  /**
   * @brief updateFilterParameters Emitted when the Filter requests all the latest Filter parameters
   * be pushed from a user-facing control (such as a widget)
   * @param filter Filter instance pointer
   */
  void updateFilterParameters(AbstractFilter* filter);

  /**
   * @brief parametersChanged Emitted when any Filter parameter is changed internally
   */
  void parametersChanged();

  /**
   * @brief preflightAboutToExecute Emitted just before calling dataCheck()
   */
  void preflightAboutToExecute();

  /**
   * @brief preflightExecuted Emitted just after calling dataCheck()
   */
  void preflightExecuted();

protected:
  FindGroupingDensity();

  /**
   * @brief dataCheck Checks for the appropriate parameter values and availability of arrays
   */
  void dataCheck();

  /**
   * @brief Initializes all the private instance variables.
   */
  void initialize();

  /**
   * @brief findSizes Determines the size of each Feature independent of geometry
   * @param igeom
   */
  void findSizes(IGeometry::Pointer igeom);

  /**
   * @brief findSizesImage Specialization for determining Feature sizes on ImageGeometries
   * @param image
   */
  void findSizesImage(ImageGeom::Pointer image);

  /**
   * @brief findSizesUnstructured Determines the size of each Feature for non-ImageGeometries
   * @param igeom
   */
  void findSizesUnstructured(IGeometry::Pointer igeom);

private:
  DEFINE_DATAARRAY_VARIABLE(float, Volumes)
  DEFINE_DATAARRAY_VARIABLE(int32_t, ParentIds)
  DEFINE_DATAARRAY_VARIABLE(float, ParentVolumes)
  DEFINE_DATAARRAY_VARIABLE(float, ParentDensities)
  DEFINE_DATAARRAY_VARIABLE(int32_t, CheckedFeatures)
  NeighborList<int32_t>::WeakPointer m_ContiguousNeighborList;
  NeighborList<int32_t>::WeakPointer m_NonContiguousNeighborList;

  FindGroupingDensity(const FindGroupingDensity&);      // Copy Constructor Not Implemented
  void operator=(const FindGroupingDensity&) = delete; // Operator '=' Not Implemented
};

#endif /* FINDSIZES_H_ */
