

#include "QtFilter.h"


class SegmentGrainsData : public QSharedData
{
  public:
    SegmentGrainsData()
      : QSharedData(), m_Misorientation(0.0f), m_InputFile("Foo")
    { }

    SegmentGrainsData(const SegmentGrainsData &other)
      : QSharedData(other), m_Misorientation(0.0f), m_InputFile("Foo")
    { }

    ~SegmentGrainsData() { }

    float m_Misorientation;
    QString m_InputFile;
};


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SegmentGrains::SegmentGrains()
{
  d_ptr = new SegmentGrainsData;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SegmentGrains::SegmentGrains(const SegmentGrains &other)
  : d_ptr (other.d_ptr)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
SegmentGrains::~SegmentGrains()
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void SegmentGrains::deepCopy(SegmentGrains& s)
{
  s.d_ptr.detach();
  s.d_ptr->m_Misorientation = d_ptr->m_Misorientation;
  s.d_ptr->m_InputFile = d_ptr->m_InputFile;
}

void SegmentGrains::copyFrom(const SegmentGrains& rhs)
{
  d_ptr->m_Misorientation = rhs.d_ptr->m_Misorientation;
  d_ptr->m_InputFile = rhs.d_ptr->m_InputFile;
}

DREAM3D_FILTER_PROPERTY_DEFN(float, Misorientation, SegmentGrains)

DREAM3D_FILTER_PROPERTY_DEFN(QString, InputFile, SegmentGrains)

