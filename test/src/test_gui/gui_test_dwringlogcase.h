TEST(DwRingLog, log)
{
    void *p = NULL;
    DwVariant v;
    DW_UI_LOGER() << v  << "\n";
    DW_UI_LOGER() << DwVariant(p)  << "\n";
    DW_UI_LOGER() << p  << "\n";
    DW_UI_LOGER() << char('a') << "\n";
    DW_UI_LOGER() << unsigned char('a') << "\n";
    DW_UI_LOGER() << wchar_t('a') << "\n";
    DW_UI_LOGER() << "char array" << "\n";
    DW_UI_LOGER() << L"wchar_t array" << "\n";
    DW_UI_LOGER() << int(1) << "\n";
    DW_UI_LOGER() << unsigned int(1) << "\n";
    DW_UI_LOGER() << short(1) << "\n";
    DW_UI_LOGER() << unsigned short(1) << "\n";
    DW_UI_LOGER() << long(1) << "\n";
    DW_UI_LOGER() << unsigned long(1) << "\n";
    DW_UI_LOGER() << float(1.0f) << "\n";
    DW_UI_LOGER() << double(1.0) << "\n";
    DW_UI_LOGER() << int64_t(1) << "\n";
    DW_UI_LOGER() << uint64_t(1) << "\n";
    DW_UI_LOGER() << ((void*)(0x00ff00ff)) << "\n";
    DW_UI_LOGER() << true << "\n";
    DW_UI_LOGER() << DwString("bbbbb") << "\n";
    DW_UI_LOGER() << DwRectF() << "\n";
    DW_UI_LOGER() << DwRectF(1, 1, 1, 1) << "\n";
    DW_UI_LOGER() << DwSizeF(1, 1) << "\n";
    DW_UI_LOGER() << DwSize(1, 1) << "\n";
    DW_UI_LOGER() << DwPointF(1, 1) << "\n";
    DW_UI_LOGER() << DwPoint(1, 1) << "\n";
    DW_UI_LOGER() << DwMatrix() << "\n";
    DW_UI_LOGER() << DwTransform() << "\n";
}