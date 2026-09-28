#include <gtk/gtk.h>
#include "main_window.h"

static MainWindow *g_main_window = nullptr;

static void on_activate(GtkApplication *app, gpointer)
{
    if (!g_main_window)
        g_main_window = new MainWindow(app);
    g_main_window->present();
}

int main(int argc, char *argv[])
{
    GtkApplication *app =
        gtk_application_new("com.example.gtk4demo", G_APPLICATION_DEFAULT_FLAGS);

    GSimpleAction *quit = g_simple_action_new("quit", nullptr);
    g_signal_connect_swapped(quit, "activate", G_CALLBACK(g_application_quit), app);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(quit));
    g_object_unref(quit);

    g_signal_connect(app, "activate", G_CALLBACK(on_activate), nullptr);

    int status = g_application_run(G_APPLICATION(app), argc, argv);

    delete g_main_window;
    g_object_unref(app);
    return status;
}
