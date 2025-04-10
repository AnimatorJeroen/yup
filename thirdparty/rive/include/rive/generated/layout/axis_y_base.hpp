#ifndef _RIVE_AXIS_YBASE_HPP_
#define _RIVE_AXIS_YBASE_HPP_
#include "rive/layout/axis.hpp"
namespace rive
{
class AxisYBase : public Axis
{
protected:
    typedef Axis Super;

public:
    static const uint16_t typeKey = 494;

    /// Helper to quickly determine if a core object extends another without
    /// RTTI at runtime.
    bool isTypeOf(uint16_t typeKey) const override
    {
        switch (typeKey)
        {
            case AxisYBase::typeKey:
            case AxisBase::typeKey:
            case ComponentBase::typeKey:
                return true;
            default:
                return false;
        }
    }

    uint16_t coreType() const override { return typeKey; }

    Core* clone() const override;

protected:
};
} // namespace rive

#endif