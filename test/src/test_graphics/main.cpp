#include "stable.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <dwcore.h>
#include "dwgraphics.h"
#include "dwgui/dwapplication.h"
#include "dwgui/dwwindow.h"
#include "dwgui/dwtimer.h"
#include "dwgui/dwresourceinterface.h"

#include "gtest_helper.h"

DwArrayList<int> t;
double fps()
{
    t.push_back(GetTickCount());

    if(t.back() - t.front() > 1000)
    {
        t.pop_front();
    }

    int begin = t.front();
    int end = t.back();

    return t.size() * 1000.0 / (end - begin + 1);
}

class DwFontDatabase
{
public:
    DwFontDatabase()
    {
        init();
    }

    ~DwFontDatabase()
    {

    }

    DwArrayList<DwString> fontNames() const
    {
        return m_fontNames;
    }

private:
    void init()
    {
        HDC hdc = GetDC(NULL);
        EnumFontFamilies(
            hdc, 
            (LPCTSTR) NULL, 
            (FONTENUMPROC) EnumFamCallBack,
            (LPARAM) this); 

        ReleaseDC(NULL, hdc);
    }

    static BOOL CALLBACK EnumFamCallBack(LPLOGFONT lplf, LPNEWTEXTMETRIC lpntm, DWORD FontType, LPVOID lParam)
    {
        DW_UNUSED(lpntm);
        DW_UNUSED(FontType);
        DwFontDatabase *pThis = static_cast<DwFontDatabase*>(lParam);
        pThis->m_fontNames.append(lplf->lfFaceName);

        return TRUE;
    }

private:
    DwArrayList<DwString> m_fontNames;
};

class Animation
{
public:
    Animation(DwImage &image, const DwPoint pt, int radius, int speed)
        : m_image(image)
    {
        m_angle = dwRand() % 360;
        m_originPpoint = pt;
        m_radius = radius;
        m_speed = speed; 
    }

    DwImage image() const { return m_image; }
    DwPoint point() const { return m_point; }
    void stepIt()
    {
        m_angle += m_speed;

        double fi = (m_angle % 360) / 360.0 * M_PI * 2 - M_PI;
        double x = m_radius * cos(fi) + m_originPpoint.x();
        double y = m_radius * sin(fi) + m_originPpoint.y();
        m_point = DwPointF(x, y).toPoint();
    }

private:
    DwImage m_image;
    DwPoint m_point;
    DwPoint m_originPpoint;
    int m_radius;
    int m_speed;
    int m_angle;
};

class MainWindow : public DwWindow
{
public:
    MainWindow()
        : m_icon(DW_PIXMAP("home:testdata/graphics_image/icon.png"))
        , m_mask(DW_PIXMAP("home:testdata/graphics_image/mask.png"))
        , m_shadow(DW_PIXMAP("home:testdata/graphics_image/shadow.png"))
    {
        for(int i = 10; i <= 30; i++)
        {
            DwString fileName = "home:testdata/graphics_image/" + DwString::number(i) + ".jpg";
            DwImage img(DW_PIXMAP(fileName));
            m_images.append(img);
        }

        for(int i = 0; i < 20; i++)
        {
            int speed = (i % 2) ? -(dwRand() % 5 + 1) : (dwRand() % 5 + 1);
            DwImage img = m_images[dwRand() % m_images.size()];
            m_anims.append(Animation(img, DwPoint(dwRand() % 1000 + 100, dwRand() % 600), dwRand() % 300 + 100, speed));
            //m_anims.append(Animation(m_icon, DwPoint(dwRand() % 1000 + 100, dwRand() % 600), dwRand() % 300 + 100, speed));
        }

        m_updateTimer.sigTimeout.connect(this, &MainWindow::on_updateTimer_timeout);
        m_updateTimer.start(10);
    }

protected:
    void on_updateTimer_timeout()
    {
        update();
    }

    void test_fileRect(DwPainter &p)
    {
        DwSize s = size();
        for(int i = 0; i < s.width(); i++)
        {
            for(int j = 0; j < s.height(); j++)
            {
                p.fillRect(DwRect(i,j, 10, 10), DwColor(dwRand() & 255, dwRand() & 255, dwRand() & 255, 64));
                j += 15;
            }
            i += 15;
        }
    }

    void test_drawLine(DwPainter &p)
    {
        DwSize s = size();
        for(int i = 0; i < s.height(); i++)
        {
            p.drawLine(DwPoint(0, i), DwPoint(s.width(), i), DwColor(0, 0, 255, i));
        }
    }

    void test_drawBackground(DwPainter &p)
    {
        DwSize s = size();
        p.fillRect(DwRect(0, 0, s.width(), s.height()), DwColor(255, 0, 0, 128));

        //p.save();
        //p.setClip(&DwRegion(DwRect(20, 20, 40, 40)));
        p.fillRect(DwRect(20, 20, 40, 40), DwColor(0, 0, 255, 255));
        p.drawRect(DwRect(20, 20, 40, 40), DwColor(0, 255, 0, 128));
        //p.restrore();
    }

    void test_drawBorder(DwPainter &p)
    {
        DwSize s = size();
        DwRect shadwoRect(0, 0, s.width(), s.height());
        p.drawBorderImage(shadwoRect, m_shadow, DwMargins(12, 12, 12, 12));
    }

    void test_drawImages(DwPainter &p)
    {
        const int KCount = 10;

        DwSize s = size();

        DwSize iconSize = m_icon.size();
        DwSize maskSize = m_mask.size();

        int xstep = s.width() / KCount;
        int ystep = s.height() / KCount;

        for(int i = 0; i < KCount; i++)
        {
            for(int j = 0; j < KCount; j++)
            {
                DwRect r1(i * xstep, j * ystep, xstep, ystep);
                DwRect r2(0, 0, iconSize.width(), iconSize.height());
                p.drawImage(i * xstep, j * ystep, m_icon);
                //p.drawImageScaled(r1, m_icon, r2);
            }
        }
    }

    void test_drawAnimations(DwPainter &p)
    {
        for(int i = 0; i < m_anims.size(); i++)
        {
            m_anims[i].stepIt();
            DwPoint pt = m_anims[i].point();
            p.drawImage(pt.x(), pt.y(), m_anims[i].image());
            p.drawBorderImage(DwRect(pt, m_anims[i].image().size()), m_shadow, DwMargins(12, 12, 12, 12));
        }
    }

    void test_drawText(DwPainter &p)
    {
		p.drawText(DwRect(100, 100, 400, 400), L"测试字体THT-=1233456789!@#$%^&*()_+", L"msyh", 42);
		return;
        int xcount = 1280 / 16;
        wchar_t str[100];
        for(int i = 0; i < 50; i++)
        {
            for(int n = 0; n < 80; n++)
            {
                str[n] = 0x3000 + i * xcount + n;
            }
            str[xcount] = 0;
            DwString s(str, xcount);

            p.drawText(DwRect(0, 16 * i, 1280, 16), s);
        }
    }

    void test_fonts(DwPainter &p)
    {
        static int scroll = 0;

        DwArrayList<DwString> fontNames = m_fontDatabase.fontNames();
        int line = 25;
        int fontCount = fontNames.count();

        for(int i = 0; i < line; i++)
        {
            DwRect r(20, i * 30 + scroll % 30, 1000, 30);
            DwString s = fontNames[(scroll + i) % fontCount] + L"测试看到法律快递费啊发动机菲拉斯发生发发发";
            p.drawText(r, s, fontNames[(scroll + i) % fontCount], 20);
        }
//        scroll++;
    }

    void test_clip(DwPainter &p)
    {
        DwTransform transform;
        transform.translate(100, 100);
        transform.rotate(30);
        p.setWorldTransform(transform);

        DwRect clipRect(5, 5, 50, 50);
        p.setClipRect(clipRect);
        p.fillRect(DwRect(0, 0, 100, 100), DwColor(64, 128, 64));

        DwTransform transform2;
        transform2.translate(0, 0);
        p.setWorldTransform(transform2);
        p.fillRect(DwRect(100, 100, 100, 100), DwColor(256, 0, 64));
        p.fillRect(DwRect(5, 5, 50, 50), DwColor(0, 0, 256));
    }

    void paintEvent(DwPainter &p, const DwRectF &rect)
    {
        DW_UNUSED(rect);

        test_drawBackground(p);

        /*test_fileRect(p);

        test_drawLine(p);

        test_drawBorder(p);

        test_drawImages(p);

        test_drawAnimations(p);*/

        //test_drawText(p);

        //test_fonts(p);

        //test_clip(p);

        dwDebug() << fps();
    }

private:
    DwVector<DwImage> m_images;
    DwImage m_icon;
    DwImage m_mask;
    DwImage m_shadow;

    DwVector<Animation> m_anims;

    DwFontDatabase m_fontDatabase;

    DwTimer m_updateTimer;
};

//TEST(DwWindow, winid)
void Test_DwWindow_winid(DwWindow *w)
{
	EXPECT_TRUE(w);
	const DwString id = w->winid();
	EXPECT_TRUE(id.startsWith(L'w'));
	DwString dummy(L"0123456789ABCDEFabcdef");
	for (int i = 1; i < id.size(); ++ i)
	{
		wchar_t c = id.at(i);
		EXPECT_TRUE(dummy.indexOf(c) != -1);
	}
}

int wmain(int argc, wchar_t* argv[])
{
	DwApplication app;

    MainWindow mw;
	mw.show();

	Test_DwWindow_winid(&mw);

    app.exec();
    
    testing::InitGoogleTest(&argc, &(argv[0]));

    RUN_ALL_TESTS();
}
