#ifndef breezestyle_h
#define breezestyle_h

//////////////////////////////////////////////////////////////////////////////
// breezestyle.h
// Breeze widget style for KDE Frameworks
// -------------------
//
// Copyright (C) 2014 Hugo Pereira Da Costa <hugo.pereira@free.fr>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License version 2 as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with this library; see the file COPYING.LIB.  If not, write to
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
//////////////////////////////////////////////////////////////////////////////

#include "breezemetrics.h"

#include <KStyle>

#include <QDockWidget>
#include <QIcon>
#include <QLoggingCategory>
#include <QStyleOption>
#include <QStylePlugin>
#include <QWidget>

namespace BreezePrivate
{
    class TabBarData;
}

namespace Breeze
{

    class Animations;
    class FrameShadowFactory;
    class Helper;
    class Mnemonics;
    class WindowManager;

    class StylePlugin : public QStylePlugin
    {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QStyleFactoryInterface" FILE "breeze.json" )

        public:

        //! constructor
        StylePlugin(QObject *parent = 0):
            QStylePlugin(parent)
        {}

        //! destructor
        ~StylePlugin();

        //! create style
        QStyle* create( const QString& );

    };

    //! base class for breeze style
    /*! it is responsible to draw all the primitives to be displayed on screen, on request from Qt paint engine */
    class Style: public KStyle
    {
        Q_OBJECT

        /* this tells kde applications that custom style elements are supported, using the kstyle mechanism */
        Q_CLASSINFO ("X-KDE-CustomElements", "true")

        public:

        //! constructor
        explicit Style( void );

        //! destructor
        virtual ~Style( void );

        //! widget polishing
        virtual void polish( QWidget* );

        //! widget unpolishing
        virtual void unpolish( QWidget* );

        //! needed to avoid warnings at compilation time
        using  KStyle::polish;
        using  KStyle::unpolish;

        //! pixel metrics
        virtual int pixelMetric(PixelMetric, const QStyleOption* = 0, const QWidget* = 0) const;

        //! style hints
        virtual int styleHint(StyleHint, const QStyleOption* = 0, const QWidget* = 0, QStyleHintReturn* = 0) const;

        //! returns rect corresponding to one widget's subelement
        virtual QRect subElementRect( SubElement subRect, const QStyleOption*, const QWidget* ) const;

        //! returns rect corresponding to one widget's subcontrol
        virtual QRect subControlRect( ComplexControl, const QStyleOptionComplex*, SubControl, const QWidget* ) const;

        //! returns size matching contents
        QSize sizeFromContents( ContentsType, const QStyleOption*, const QSize&, const QWidget* ) const;

        //! returns which subcontrol given QPoint corresponds to
        SubControl hitTestComplexControl( ComplexControl, const QStyleOptionComplex*, const QPoint&, const QWidget* ) const;

        //! primitives
        void drawPrimitive( PrimitiveElement, const QStyleOption*, QPainter*, const QWidget* ) const;

        //! controls
        void drawControl( ControlElement, const QStyleOption*, QPainter*, const QWidget* ) const;

        //! complex controls
        void drawComplexControl( ComplexControl, const QStyleOptionComplex*, QPainter*, const QWidget* ) const;

        //! generic text rendering
        virtual void drawItemText(
            QPainter*, const QRect&, int alignment, const QPalette&, bool enabled,
            const QString&, QPalette::ColorRole = QPalette::NoRole) const;

        //!@name event filters
        //@{

        virtual bool eventFilter(QObject *, QEvent *);

        //! dock widget
        /*! needed to render proper frame */
        bool eventFilterDockWidget( QDockWidget*, QEvent* );

        //! install event filter to object, in a unique way
        void addEventFilter( QObject* object )
        {
            object->removeEventFilter( this );
            object->installEventFilter( this );
        }

        //@}


        protected Q_SLOTS:

        //! update breeze configuration
        void configurationChanged( void );

        protected:

//         //! standard icons
//         virtual QIcon standardIcon( StandardPixmap, const QStyleOption* = 0, const QWidget* = 0) const;

        //! load configuration
        void loadConfiguration();

        //!@name enumerations and convenience classes
        //@{

        //! arrow orientation
        enum ArrowOrientation
        {
            ArrowNone,
            ArrowUp,
            ArrowDown,
            ArrowLeft,
            ArrowRight
        };


        //! get polygon corresponding to generic arrow
        enum ArrowSize
        {
            ArrowNormal,
            ArrowSmall,
            ArrowTiny
        };

        //@}

        //!@name subelementRect specialized functions
        //@{

        //! default implementation. Does not change anything
        QRect defaultSubElementRect( const QStyleOption* option, const QWidget* ) const
        { return option->rect; }

//         //! pushbutton contents
//         QRect pushButtonContentsRect( const QStyleOption* option, const QWidget* ) const;
//
//         //! toolbox tab
//         QRect toolBoxTabContentsRect( const QStyleOption* option, const QWidget* ) const;
//
        //! checkbox contents
        QRect checkBoxContentsRect( const QStyleOption* option, const QWidget* ) const
        { return handleRTL( option, option->rect.adjusted( Metrics::CheckBox_Size + Metrics::CheckBox_BoxTextSpace, 0, 0, 0 ) ); }

        //! checkbox focus
        QRect checkBoxFocusRect( const QStyleOption*, const QWidget* ) const;

        //! radio button contents
        QRect radioButtonContentsRect( const QStyleOption* option, const QWidget* widget ) const
        { return checkBoxContentsRect( option, widget ); }

        //! radio button focus
        QRect radioButtonFocusRect( const QStyleOption* option, const QWidget* widget ) const
        { return checkBoxFocusRect( option, widget ); }

        //! line edit contents
        QRect lineEditContentsRect( const QStyleOption*, const QWidget* ) const;

        //! progressbar groove
        QRect progressBarGrooveRect( const QStyleOption* option, const QWidget* ) const;

        //! progressbar groove
        QRect progressBarContentsRect( const QStyleOption* option, const QWidget* ) const;

        //! progressbar label
        QRect progressBarLabelRect( const QStyleOption* option, const QWidget* ) const;

//         //! tabBar buttons
//         QRect tabBarTabLeftButtonRect( const QStyleOption* option, const QWidget* widget ) const;
//
//         QRect tabBarTabRightButtonRect( const QStyleOption* option, const QWidget* widget ) const;
//
//         QRect tabBarTabButtonRect( SubElement, const QStyleOption*, const QWidget* ) const;
//
//         // tabbar tab text
//         QRect tabBarTabTextRect( const QStyleOption* option, const QWidget* widget ) const;
//

        // view headers
        QRect headerArrowRect( const QStyleOption* option, const QWidget* ) const;
        QRect headerLabelRect( const QStyleOption* option, const QWidget* ) const;

        // tab widgets
        QRect tabWidgetTabBarRect( const QStyleOption*, const QWidget* ) const;
        QRect tabWidgetTabContentsRect( const QStyleOption*, const QWidget* ) const;
        QRect tabWidgetTabPaneRect( const QStyleOption*, const QWidget* ) const;
        QRect tabWidgetCornerRect( SubElement, const QStyleOption* option, const QWidget* widget ) const;

        //@}

        //!@name subcontrol Rect specialized functions
        //@{

        QRect groupBoxSubControlRect( const QStyleOptionComplex*, SubControl, const QWidget* ) const;
        QRect comboBoxSubControlRect( const QStyleOptionComplex*, SubControl, const QWidget* ) const;
        QRect spinBoxSubControlRect( const QStyleOptionComplex*, SubControl, const QWidget* ) const;
        QRect scrollBarInternalSubControlRect( const QStyleOptionComplex*, SubControl ) const;
        QRect scrollBarSubControlRect( const QStyleOptionComplex*, SubControl, const QWidget* ) const;
        QRect dialSubControlRect( const QStyleOptionComplex*, SubControl, const QWidget* ) const;

        //@}

        //!@name sizeFromContents
        //@{
        QSize defaultSizeFromContents( const QStyleOption*, const QSize& size, const QWidget* ) const
        { return size; }

        QSize checkBoxSizeFromContents( const QStyleOption*, const QSize&, const QWidget* ) const;
        QSize lineEditSizeFromContents( const QStyleOption*, const QSize&, const QWidget* ) const;
        QSize comboBoxSizeFromContents( const QStyleOption*, const QSize&, const QWidget* ) const;
        QSize spinBoxSizeFromContents( const QStyleOption*, const QSize&, const QWidget* ) const;
        QSize pushButtonSizeFromContents( const QStyleOption*, const QSize&, const QWidget* ) const;
        QSize menuBarItemSizeFromContents( const QStyleOption*, const QSize& size, const QWidget* ) const;
        QSize menuItemSizeFromContents( const QStyleOption*, const QSize& size, const QWidget* ) const;

//         QSize menuBarSizeFromContents( const QStyleOption*, const QSize& size, const QWidget* ) const;
//         QSize menuItemSizeFromContents( const QStyleOption*, const QSize&, const QWidget* ) const;
        QSize progressBarSizeFromContents( const QStyleOption*, const QSize&, const QWidget* ) const;
        QSize tabWidgetSizeFromContents( const QStyleOption*, const QSize& size, const QWidget* ) const;
        QSize tabBarTabSizeFromContents( const QStyleOption*, const QSize& size, const QWidget* ) const;
//         QSize tabWidgetSizeFromContents( const QStyleOption*, const QSize& size, const QWidget* ) const;
//         QSize toolButtonSizeFromContents( const QStyleOption*, const QSize&, const QWidget* ) const;
        QSize headerSectionSizeFromContents( const QStyleOption*, const QSize&, const QWidget* ) const;

        //@}

        //!@name primitives specialized functions
        //@{

        bool emptyPrimitive( const QStyleOption*, QPainter*, const QWidget* ) const
        { return true; }

        bool drawFramePrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;
        bool drawFrameGroupBoxPrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;
//         bool drawFrameMenuPrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;
        bool drawFrameTabWidgetPrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;
        bool drawFrameTabBarBasePrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;
        bool drawFrameFocusRectPrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;
//         bool drawFrameWindowPrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;
//         bool drawIndicatorTabClose( const QStyleOption* option, QPainter* painter, const QWidget* widget ) const;

        bool drawIndicatorArrowUpPrimitive( const QStyleOption* option, QPainter* painter, const QWidget* widget ) const
        { return drawIndicatorArrowPrimitive( ArrowUp, option, painter, widget ); }

        bool drawIndicatorArrowDownPrimitive( const QStyleOption* option, QPainter* painter, const QWidget* widget ) const
        { return drawIndicatorArrowPrimitive( ArrowDown, option, painter, widget ); }

        bool drawIndicatorArrowLeftPrimitive( const QStyleOption* option, QPainter* painter, const QWidget* widget ) const
        { return drawIndicatorArrowPrimitive( ArrowLeft, option, painter, widget ); }

        bool drawIndicatorArrowRightPrimitive( const QStyleOption* option, QPainter* painter, const QWidget* widget ) const
        { return drawIndicatorArrowPrimitive( ArrowRight, option, painter, widget ); }

        bool drawIndicatorArrowPrimitive( ArrowOrientation, const QStyleOption*, QPainter*, const QWidget* ) const;
        bool drawIndicatorHeaderArrowPrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;

//         //! dock widget separators
//         /*! it uses the same painting as QSplitter, but due to Qt, the horizontal/vertical convention is inverted */
//         bool drawIndicatorDockWidgetResizeHandlePrimitive( const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        bool drawPanelButtonCommandPrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;
        bool drawPanelButtonToolPrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;
//         bool drawPanelMenuPrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;
//         bool drawPanelScrollAreaCornerPrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;
//         bool drawPanelTipLabelPrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;
//         bool drawPanelItemViewItemPrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;
//         bool drawPanelLineEditPrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;
//         bool drawIndicatorMenuCheckMarkPrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;
//         bool drawIndicatorBranchPrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;
//         bool drawIndicatorButtonDropDownPrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;
        bool drawIndicatorCheckBoxPrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;
        bool drawIndicatorRadioButtonPrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;
//         bool drawIndicatorTabTearPrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;
//         bool drawIndicatorToolBarHandlePrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;
        bool drawIndicatorToolBarSeparatorPrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;
//         bool drawWidgetPrimitive( const QStyleOption*, QPainter*, const QWidget* ) const;

        //@}

        //!@name controls specialized functions
        //@{

        bool emptyControl( const QStyleOption*, QPainter*, const QWidget* ) const
        { return true; }

//         virtual bool drawCapacityBarControl( const QStyleOption*, QPainter*, const QWidget* ) const;
        virtual bool drawComboBoxLabelControl( const QStyleOption*, QPainter*, const QWidget* ) const;
//         virtual bool drawDockWidgetTitleControl( const QStyleOption*, QPainter*, const QWidget* ) const;
        virtual bool drawMenuBarItemControl( const QStyleOption*, QPainter*, const QWidget* ) const;
        virtual bool drawMenuItemControl( const QStyleOption*, QPainter*, const QWidget* ) const;
        virtual bool drawProgressBarControl( const QStyleOption*, QPainter*, const QWidget* ) const;
        virtual bool drawProgressBarContentsControl( const QStyleOption*, QPainter*, const QWidget* ) const;
        virtual bool drawProgressBarGrooveControl( const QStyleOption*, QPainter*, const QWidget* ) const;
        virtual bool drawProgressBarLabelControl( const QStyleOption*, QPainter*, const QWidget* ) const;
        virtual bool drawPushButtonLabelControl( const QStyleOption*, QPainter*, const QWidget* ) const;
//
        //! scrollbar
        virtual bool drawScrollBarSliderControl( const QStyleOption*, QPainter*, const QWidget* ) const;
        virtual bool drawScrollBarAddLineControl( const QStyleOption*, QPainter*, const QWidget* ) const;
        virtual bool drawScrollBarSubLineControl( const QStyleOption*, QPainter*, const QWidget* ) const;
        virtual bool drawScrollBarAddPageControl( const QStyleOption*, QPainter*, const QWidget* ) const;
        virtual bool drawScrollBarSubPageControl( const QStyleOption*, QPainter*, const QWidget* ) const;

        virtual bool drawShapedFrameControl( const QStyleOption*, QPainter*, const QWidget* ) const;
        virtual bool drawRubberBandControl( const QStyleOption*, QPainter*, const QWidget* ) const;
        virtual bool drawHeaderSectionControl( const QStyleOption*, QPainter*, const QWidget* ) const;
        virtual bool drawHeaderEmptyAreaControl( const QStyleOption*, QPainter*, const QWidget* ) const;
//         virtual bool drawHeaderLabelControl( const QStyleOption*, QPainter*, const QWidget* ) const;
//         virtual bool drawSplitterControl( const QStyleOption* option, QPainter* painter, const QWidget* widget ) const;

        //! tabbar tabs.
        virtual bool drawTabBarTabLabelControl( const QStyleOption*, QPainter*, const QWidget* ) const;
        virtual bool drawTabBarTabShapeControl( const QStyleOption*, QPainter*, const QWidget* ) const;
//         virtual bool drawToolBarControl( const QStyleOption*, QPainter*, const QWidget* ) const;
//         virtual bool drawToolBoxTabLabelControl( const QStyleOption*, QPainter*, const QWidget* ) const;
//         virtual bool drawToolBoxTabShapeControl( const QStyleOption*, QPainter*, const QWidget* ) const;
//         virtual bool drawToolButtonLabelControl( const QStyleOption*, QPainter*, const QWidget* ) const;

        //!@}

        //!@name complex ontrols specialized functions
        //@{
        bool drawComboBoxComplexControl( const QStyleOptionComplex*, QPainter*, const QWidget* ) const;
        bool drawSpinBoxComplexControl( const QStyleOptionComplex*, QPainter*, const QWidget* ) const;
        bool drawSliderComplexControl( const QStyleOptionComplex*, QPainter*, const QWidget* ) const;
        bool drawDialComplexControl( const QStyleOptionComplex*, QPainter*, const QWidget* ) const;
//         bool drawTitleBarComplexControl( const QStyleOptionComplex*, QPainter*, const QWidget* ) const;
//         bool drawToolButtonComplexControl( const QStyleOptionComplex*, QPainter*, const QWidget* ) const;
        //@}

        //! adjust rect based on provided margins
        QRect insideMargin( const QRect& r, int margin ) const
        { return insideMargin( r, margin, margin ); }

        //! adjust rect based on provided margins
        QRect insideMargin( const QRect& r, int marginWidth, int marginHeight ) const
        { return r.adjusted( marginWidth, marginHeight, -marginWidth, -marginHeight ); }

        //! expand size based on margins
        QSize expandSize( const QSize& size, int margin ) const
        { return expandSize( size, margin, margin ); }

        //! expand size based on margins
        QSize expandSize( const QSize& size, int marginWidth, int marginHeight ) const
        { return size + 2*QSize( marginWidth, marginHeight ); }

        //! returns true for vertical tabs
        bool isVerticalTab( const QStyleOptionTab* option ) const
        { return isVerticalTab( option->shape ); }

        bool isVerticalTab( const QTabBar::Shape& shape ) const
        {
            return shape == QTabBar::RoundedEast
                || shape == QTabBar::RoundedWest
                || shape == QTabBar::TriangularEast
                || shape == QTabBar::TriangularWest;

        }

        //! returns true for reflected tabs
        bool isReflected( const QStyleOptionTab* option ) const
        { return isReflected( option->shape ); }

        bool isReflected( const QTabBar::Shape& shape ) const
        {
            return shape == QTabBar::RoundedEast
                || shape == QTabBar::RoundedSouth
                || shape == QTabBar::TriangularEast
                || shape == QTabBar::TriangularSouth;

        }

        //! right to left alignment handling
        QRect handleRTL(const QStyleOption* opt, const QRect& subRect) const
        { return visualRect(opt->direction, opt->rect, subRect); }

        //! right to left alignment handling
        QPoint handleRTL(const QStyleOption* opt, const QPoint& pos) const
        { return visualPos(opt->direction, opt->rect, pos); }

        QRect centerRect(const QRect &in, const QSize& s ) const
        { return centerRect( in, s.width(), s.height() ); }

        QRect centerRect(const QRect &in, int w, int h) const
        { return QRect(in.x() + (in.width() - w)/2, in.y() + (in.height() - h)/2, w, h); }

        //! return dial angle based on option and value
        qreal dialAngle( const QStyleOptionSlider*, int ) const;

        /*
        Checks whether the point is before the bound rect for bound of given orientation.
        This is needed to implement custom number of buttons in scrollbars,
        as well as proper mouse-hover
        */
        inline bool preceeds( const QPoint&, const QRect&, const QStyleOption* ) const;

        //! return which arrow button is hit by point for scrollbar double buttons
        inline QStyle::SubControl scrollBarHitTest( const QRect&, const QPoint&, const QStyleOption* ) const;

        //! returns relevant scrollbar parent
        /*! needed to detect parent focus */
        QWidget* scrollBarParent( const QWidget* ) const;

        //! returns true if given scrollbar arrow is animated
        QColor scrollBarArrowColor( const QStyleOptionSlider*, const SubControl&, const QWidget* ) const;

        //! returns point position for generic arrows
        QPolygonF genericArrow( ArrowOrientation, ArrowSize = ArrowNormal ) const;

        //! scrollbar arrow
        void renderScrollBarArrow( QPainter*, const QRect&, const QColor& color, ArrowOrientation ) const;

        //! spinbox arrows
        void renderSpinBoxArrow( QPainter*, const QStyleOptionSpinBox*, const QWidget*, const SubControl& ) const;

        //! scrollbar buttons
        enum ScrollBarButtonType
        {
            NoButton,
            SingleButton,
            DoubleButton
        };

        //! returns height for scrollbar buttons depending of button types
        int scrollBarButtonHeight( const ScrollBarButtonType& type ) const
        {
            switch( type )
            {
                case NoButton: return Metrics::ScrollBar_NoButtonHeight;
                case SingleButton: return ScrollBar_SingleButtonHeight;
                case DoubleButton: return ScrollBar_DoubleButtonHeight;
                default: return 0;
            }
        }

        //@}

        private:

        //!@name scrollbar button types (for addLine and subLine )
        //@{
        ScrollBarButtonType _addLineButtons;
        ScrollBarButtonType _subLineButtons;
        //@}

        //! helper
        Helper* _helper;

        //! animations
        Animations* _animations;

        //! keyboard accelerators
        Mnemonics* _mnemonics;

        //! tabbar data
        BreezePrivate::TabBarData* _tabBarData;

        //! window manager
        WindowManager* _windowManager;

        //! frame shadows
        FrameShadowFactory* _frameShadowFactory;

        //! pointer to primitive specialized function
        typedef bool (Style::*StylePrimitive)( const QStyleOption*, QPainter*, const QWidget* ) const;

        //! pointer to control specialized function
        typedef bool (Style::*StyleControl)( const QStyleOption*, QPainter*, const QWidget* ) const;

        //! pointer to control specialized function
        typedef bool (Style::*StyleComplexControl)( const QStyleOptionComplex*, QPainter*, const QWidget* ) const;

        //!@name custom elements
        //@{

        //! use Argb Drag and Drop Window
        QStyle::StyleHint SH_ArgbDndWindow;

        //! styled painting for KCapacityBar
        QStyle::ControlElement CE_CapacityBar;

        //@}

    };

    //_________________________________________________________________________
    bool Style::preceeds( const QPoint& point, const QRect& bound, const QStyleOption* option ) const
    {
        if( option->state&QStyle::State_Horizontal)
        {

            if( option->direction == Qt::LeftToRight) return point.x() < bound.right();
            else return point.x() > bound.x();

        } else return point.y() < bound.y();

    }

    //_________________________________________________________________________
    QStyle::SubControl Style::scrollBarHitTest( const QRect& rect, const QPoint& point, const QStyleOption* option ) const
    {
        if( option->state & QStyle::State_Horizontal)
        {

            if( option->direction == Qt::LeftToRight ) return point.x() < rect.center().x() ? QStyle::SC_ScrollBarSubLine : QStyle::SC_ScrollBarAddLine;
            else return point.x() > rect.center().x() ? QStyle::SC_ScrollBarSubLine : QStyle::SC_ScrollBarAddLine;

        } else return point.y() < rect.center().y() ? QStyle::SC_ScrollBarSubLine : QStyle::SC_ScrollBarAddLine;

    }

}

Q_DECLARE_LOGGING_CATEGORY(BREEZE )

#endif
