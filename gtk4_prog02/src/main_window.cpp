#include "main_window.h"

#define RESOURCE_BASE "/com/example/gtk4demo"

MainWindow::MainWindow(GtkApplication *app)
{
    m_builder = gtk_builder_new_from_resource(RESOURCE_BASE "/ui/main_window.ui");
    m_widget  = GTK_WIDGET(gtk_builder_get_object(m_builder, "main_window"));
    gtk_window_set_application(GTK_WINDOW(m_widget), app);
    connect_actions();
}

MainWindow::~MainWindow()
{
    if (m_builder)       g_object_unref(m_builder);
    if (m_about_builder) g_object_unref(m_about_builder);
    if (m_info_builder)  g_object_unref(m_info_builder);
    delete m_settings;
}

void MainWindow::present()
{
    gtk_window_present(GTK_WINDOW(m_widget));
}

void MainWindow::setParams(std::string params)
{
    m_editTestText = params;
    g_print("Test Edit text: %s\n", m_editTestText.c_str());
}

void MainWindow::connect_actions()
{
    auto add = [&](const char *name, GCallback cb) {
        GSimpleAction *a = g_simple_action_new(name, nullptr);
        g_signal_connect(a, "activate", cb, this);
        g_action_map_add_action(G_ACTION_MAP(m_widget), G_ACTION(a));
        g_object_unref(a);
    };
    add("show-about",    G_CALLBACK(on_show_about));
    add("show-settings", G_CALLBACK(on_show_settings));
    add("show-info",     G_CALLBACK(on_show_info));
}

void MainWindow::show_about()
{
    if (!m_about_win) {
        m_about_builder = gtk_builder_new_from_resource(RESOURCE_BASE "/ui/about_window.ui");
        m_about_win     = GTK_WIDGET(gtk_builder_get_object(m_about_builder, "about_window"));
        gtk_window_set_transient_for(GTK_WINDOW(m_about_win), GTK_WINDOW(m_widget));
    }
    gtk_window_present(GTK_WINDOW(m_about_win));
}

void MainWindow::show_settings()
{
    if (!m_settings)
        m_settings = new SettingsWindow(GTK_WINDOW(m_widget), this);
    m_settings->setTestEntry(m_editTestText);
    m_settings->present();
}

void MainWindow::show_info()
{
    if (!m_info_win) {
        m_info_builder = gtk_builder_new_from_resource(RESOURCE_BASE "/ui/info_window.ui");
        m_info_win     = GTK_WIDGET(gtk_builder_get_object(m_info_builder, "info_window"));
        gtk_window_set_transient_for(GTK_WINDOW(m_info_win), GTK_WINDOW(m_widget));

        char version[32];
        g_snprintf(version, sizeof(version), "%u.%u.%u",
                   gtk_get_major_version(),
                   gtk_get_minor_version(),
                   gtk_get_micro_version());
        GtkLabel *lbl = GTK_LABEL(gtk_builder_get_object(m_info_builder, "info_gtk_value"));
        gtk_label_set_text(lbl, version);
    }
    gtk_window_present(GTK_WINDOW(m_info_win));
}

void MainWindow::on_show_about(GSimpleAction *, GVariant *, gpointer self)
{
    static_cast<MainWindow *>(self)->show_about();
}

void MainWindow::on_show_settings(GSimpleAction *, GVariant *, gpointer self)
{
    static_cast<MainWindow *>(self)->show_settings();
}

void MainWindow::on_show_info(GSimpleAction *, GVariant *, gpointer self)
{
    static_cast<MainWindow *>(self)->show_info();
}
