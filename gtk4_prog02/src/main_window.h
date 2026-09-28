#pragma once
#include <gtk/gtk.h>
#include "settings_window.h"

class IMainWindow {
public:
    virtual void setParams(std::string params) = 0;
};

class MainWindow : public IMainWindow {
public:
    explicit MainWindow(GtkApplication *app);
    ~MainWindow();
    void present();
    void setParams(std::string params) override;

private:
    GtkWidget      *m_widget        = nullptr;
    GtkBuilder     *m_builder       = nullptr;

    GtkWidget      *m_about_win     = nullptr;
    SettingsWindow *m_settings      = nullptr;
    GtkWidget      *m_info_win      = nullptr;
    GtkBuilder     *m_about_builder = nullptr;
    GtkBuilder     *m_info_builder  = nullptr;

    std::string    m_editTestText = "None";

    void connect_actions();
    void show_about();
    void show_settings();
    void show_info();

    static void on_show_about   (GSimpleAction *, GVariant *, gpointer self);
    static void on_show_settings(GSimpleAction *, GVariant *, gpointer self);
    static void on_show_info    (GSimpleAction *, GVariant *, gpointer self);
};
