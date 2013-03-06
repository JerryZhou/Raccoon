#include <QtGui>

class MainWindow : public QWidget
{
	Q_OBJECT

public:
    MainWindow() : QWidget(NULL, Qt::FramelessWindowHint)
	{
        setAttribute(Qt::WA_TranslucentBackground);
        m_icon.load("../../bin/testdata/graphics_image/alpha.png");
        m_mask.load("../../bin/testdata/graphics_image/mask.png");

        QTimer *t = new QTimer(this);
        connect(t, SIGNAL(timeout()), SLOT(on_update_timeout()));
        t->start(20);
    }

protected:
    void paintEvent(QPaintEvent *)
    {
        static int offset = 0;

        QPainter p(this);
        QSize s = size();
        int xstep = s.width() / 10;
        int ystep = s.height() / 10;
        for(int i = 0; i < 10; i++)
        {
            for(int j = 0; j < 10; j++)
            {
                p.drawPixmap(offset + i * xstep, j * ystep, m_icon);
                p.drawPixmap(i * xstep, offset + j * ystep, m_mask);
            }
        }
        offset += 2;
        offset %= xstep;
    }

private slots:
    void on_update_timeout()
    {
        update();
    }

private:
    QPixmap m_icon;
    QPixmap m_mask;
};

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	MainWindow mw;
	mw.showMaximized();

	return app.exec();
}

#include "main.moc"