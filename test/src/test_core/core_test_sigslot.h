namespace
{
    class Signal
    {
    public:
        void fire()
        {
            sig0.emit();
            sig1.emit(1000);
            sig2.emit(2000, "Hello");
        }

    public:
        sigslot::signal0<> sig0;
        sigslot::signal1<int> sig1;
        sigslot::signal2<int, const DwString &> sig2;
    };

    class Slot : public sigslot::has_slots
    {
    public:
        Slot()
        {
            m_signal.sig0.connect(this, &Slot::on_sig0);
            m_signal.sig1.connect(this, &Slot::on_sig1);
            m_signal.sig2.connect(this, &Slot::on_sig2);
        }

        void test()
        {
            m_signal.fire();
        }

    private:
        void on_sig0()
        {
            
        }

        void on_sig1(int n)
        {
            EXPECT_EQ(n, 1000);
        }

        void on_sig2(int i, const DwString &s)
        {
            EXPECT_EQ(i, 2000);
            EXPECT_EQ(s, DwString("Hello"));
        }
        
    private:
        Signal m_signal;
    };
}

TEST(Sigslot, Sigslot)
{
    Slot s;

    s.test();
}

TEST(Sigslot, performance)
{
    Slot s;

    for(int i = 0; i < 10000; i++)
    {
        s.test();
    }
}
