/*
class SignalObject : public DwObject
{
public:
    void doClick()
    {
        DwParams p;
        p << 1000;
        p << 2000;
        clicked.emit(this, p);
    }
	DwSignal clicked;
};

class SlotObject : public DwObject
{
public:
    void on_button1_clicked(DwObject *sender, const DwParams &params)
    {
        DW_UNUSED(sender);
        DwVariant v = params.at(0);
        int n = params.at(0).toInt();
        EXPECT_EQ(1000, n);
    }

    void on_button2_clicked(DwObject *sender, const DwParams &params)
    {
        DW_UNUSED(sender);
        DwVariant v = params.at(0);
        int n = params.at(0).toInt();
        int m = params.at(1).toInt();
        EXPECT_EQ(1000, n);
        EXPECT_EQ(2000, m);
    }
};

TEST(SigSlot, SigSlot)
{
    SignalObject s1;
    SlotObject s2;
    s1.clicked.connect(&s2, &SlotObject::on_button1_clicked);
    s1.clicked.connect(&s2, &SlotObject::on_button2_clicked);

    s1.doClick();
}*/