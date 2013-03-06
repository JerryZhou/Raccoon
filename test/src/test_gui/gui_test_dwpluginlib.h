
TEST(DwPluginLib, enumplugins)
{
    DwPluginLib * pluginLib = dwPluginLib();
    pluginLib->setup();
    pluginLib->enumPlugins();
    const DwMap<DwString, DwPluginDesc>& plugins = pluginLib->descs();

    DW_UI_LOGER() << plugins;
}