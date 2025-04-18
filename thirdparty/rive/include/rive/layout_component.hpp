#ifndef _RIVE_LAYOUT_COMPONENT_HPP_
#define _RIVE_LAYOUT_COMPONENT_HPP_
#include "rive/animation/keyframe_interpolator.hpp"
#include "rive/drawable.hpp"
#include "rive/generated/layout_component_base.hpp"
#include "rive/layout/layout_component_style.hpp"
#include "rive/layout/layout_measure_mode.hpp"
#include "rive/math/raw_path.hpp"
#include "rive/shapes/rectangle.hpp"
#include "rive/shapes/shape_paint_container.hpp"
#ifdef WITH_RIVE_LAYOUT
#include "yoga/YGNode.h"
#include "yoga/YGStyle.h"
#include "yoga/Yoga.h"
#endif

namespace rive
{

class AABB;
class KeyFrameInterpolator;

struct LayoutData
{
#ifdef WITH_RIVE_LAYOUT
    YGNode node;
    YGStyle style;
#endif
};

class Layout
{
public:
    Layout() : m_left(0.0f), m_top(0.0f), m_width(0.0f), m_height(0.0f) {}
    Layout(float left, float top, float width, float height) :
        m_left(left), m_top(top), m_width(width), m_height(height)
    {}
#ifdef WITH_RIVE_LAYOUT
    Layout(const YGLayout& layout);
#endif

    bool operator==(const Layout& o) const
    {
        return m_left == o.m_left && m_top == o.m_top && m_width == o.m_width &&
               m_height == o.m_height;
    }
    bool operator!=(const Layout& o) const { return !(*this == o); }

    static Layout lerp(const Layout& from, const Layout& to, float f)
    {
        float fi = 1.0f - f;
        return Layout(to.m_left * f + from.m_left * fi,
                      to.m_top * f + from.m_top * fi,
                      to.m_width * f + from.m_width * fi,
                      to.m_height * f + from.m_height * fi);
    }

    float left() const { return m_left; }
    float top() const { return m_top; }
    float width() const { return m_width; }
    float height() const { return m_height; }

private:
    float m_left;
    float m_top;
    float m_width;
    float m_height;
};

struct LayoutAnimationData
{
    float elapsedSeconds = 0.0f;
    Layout from;
    Layout to;
    Layout interpolate(float f) const { return Layout::lerp(from, to, f); }
    void copy(const LayoutAnimationData& from);
};

class LayoutComponent : public LayoutComponentBase,
                        public ProxyDrawing,
                        public ShapePaintContainer,
                        public AdvancingComponent,
                        public InterpolatorHost
{
protected:
    LayoutComponentStyle* m_style = nullptr;
    std::unique_ptr<LayoutData> m_layoutData;

    Layout m_layout;

    LayoutAnimationData m_animationDataA;
    LayoutAnimationData m_animationDataB;
    bool m_isSmoothingAnimation = false;
    KeyFrameInterpolator* m_inheritedInterpolator;
    LayoutStyleInterpolation m_inheritedInterpolation =
        LayoutStyleInterpolation::hold;
    float m_inheritedInterpolationTime = 0;
    Rectangle* m_backgroundRect = new Rectangle();
    rcp<RenderPath> m_backgroundPath;
    rcp<RenderPath> m_clipPath;
    DrawableProxy m_proxy;

    Artboard* getArtboard() override { return artboard(); }
    LayoutAnimationData* currentAnimationData();

    LayoutComponent* layoutParent()
    {
        auto p = parent();
        while (p != nullptr)
        {
            if (p->is<LayoutComponent>())
            {
                return p->as<LayoutComponent>();
            }
            p = p->parent();
        }
        return nullptr;
    }

private:
    float m_widthOverride = NAN;
    int m_widthUnitValueOverride = -1;
    float m_heightOverride = NAN;
    int m_heightUnitValueOverride = -1;
    bool m_parentIsRow = true;
    bool m_widthIntrinsicallySizeOverride = false;
    bool m_heightIntrinsicallySizeOverride = false;

#ifdef WITH_RIVE_LAYOUT
protected:
    YGNode& layoutNode() { return m_layoutData->node; }
    YGStyle& layoutStyle() { return m_layoutData->style; }
    void syncLayoutChildren();
    void propagateSizeToChildren(ContainerComponent* component);
    bool applyInterpolation(float elapsedSeconds, bool animate = true);

protected:
    void calculateLayout();
#endif

public:
    LayoutComponentStyle* style() { return m_style; }
    void style(LayoutComponentStyle* style) { m_style = style; }

    void draw(Renderer* renderer) override;
    void drawProxy(Renderer* renderer) override;
    bool isProxyHidden() override { return isHidden(); }
    Core* hitTest(HitInfo*, const Mat2D&) override;
    DrawableProxy* proxy() { return &m_proxy; };
    virtual void updateRenderPath();
    void update(ComponentDirt value) override;
    void onDirty(ComponentDirt value) override;
    AABB layoutBounds()
    {
        return AABB::fromLTWH(m_layout.left(),
                              m_layout.top(),
                              m_layout.width(),
                              m_layout.height());
    }
    AABB localBounds() const override
    {
        return AABB::fromLTWH(0.0f, 0.0f, m_layout.width(), m_layout.height());
    }
    AABB worldBounds() const
    {
        auto transform = worldTransform();
        return AABB::fromLTWH(transform[4],
                              transform[5],
                              m_layout.width(),
                              m_layout.height());
    }

    // We provide a way for nested artboards (or other objects) to override this
    // layout's width/height and unit values.
    void widthOverride(float width, int unitValue = 1, bool isRow = true);
    void heightOverride(float height, int unitValue = 1, bool isRow = true);
    void widthIntrinsicallySizeOverride(bool intrinsic);
    void heightIntrinsicallySizeOverride(bool intrinsic);
    virtual bool canHaveOverrides() { return false; }
    bool mainAxisIsRow();
    bool mainAxisIsColumn();
    bool overridesKeyedInterpolation(int propertyKey) override;
    bool advanceComponent(float elapsedSeconds, bool animate = true) override;

#ifdef WITH_RIVE_LAYOUT
    LayoutComponent() :
        m_layoutData(std::unique_ptr<LayoutData>(new LayoutData())),
        m_proxy(this)
    {
        layoutNode().getConfig()->setPointScaleFactor(0);
    }
    ~LayoutComponent() { delete m_backgroundRect; }
    void syncStyle();
    virtual void propagateSize();
    void updateLayoutBounds(bool animate = true);
    StatusCode onAddedDirty(CoreContext* context) override;
    StatusCode onAddedClean(CoreContext* context) override;
    bool advance(float elapsedSeconds);
    bool animates();
    LayoutAnimationStyle animationStyle();
    KeyFrameInterpolator* interpolator();
    LayoutStyleInterpolation interpolation();
    float interpolationTime();

    void cascadeAnimationStyle(LayoutStyleInterpolation inheritedInterpolation,
                               KeyFrameInterpolator* inheritedInterpolator,
                               float inheritedInterpolationTime);
    void setInheritedInterpolation(
        LayoutStyleInterpolation inheritedInterpolation,
        KeyFrameInterpolator* inheritedInterpolator,
        float inheritedInterpolationTime);
    void clearInheritedInterpolation();
    void interruptAnimation();
    bool isLeaf();
    void positionTypeChanged();
    void scaleTypeChanged();
#else
    LayoutComponent() :
        m_layoutData(std::unique_ptr<LayoutData>(new LayoutData())),
        m_proxy(this)
    {}
#endif
    void buildDependencies() override;

    void markLayoutNodeDirty();
    void markLayoutStyleDirty();
    void clipChanged() override;
    void widthChanged() override;
    void heightChanged() override;
    void styleIdChanged() override;
    void fractionalWidthChanged() override;
    void fractionalHeightChanged() override;

    Vec2D measureLayout(float width,
                        LayoutMeasureMode widthMode,
                        float height,
                        LayoutMeasureMode heightMode) override;
};
} // namespace rive

#endif