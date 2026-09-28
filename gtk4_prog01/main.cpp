#include <gtk/gtk.h>

static void on_button_clicked(GtkButton *button, gpointer user_data) {
    GtkLabel *label = GTK_LABEL(user_data);
    gtk_label_set_text(label, "Button was clicked!");
    gtk_button_set_label(button, "Clicked!");
}

static void activate(GtkApplication *app, gpointer /*user_data*/) {
    GtkBuilder *builder = gtk_builder_new_from_resource("/org/example/hello/window.ui");

    GtkWidget *window      = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    GtkWidget *button      = GTK_WIDGET(gtk_builder_get_object(builder, "button"));
    GtkWidget *status_label = GTK_WIDGET(gtk_builder_get_object(builder, "status_label"));

    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), status_label);

    gtk_window_set_application(GTK_WINDOW(window), app);
    gtk_window_present(GTK_WINDOW(window));

    g_object_unref(builder);
}

int main(int argc, char *argv[]) {
    GtkApplication *app = gtk_application_new("org.example.hello", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), nullptr);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}