#include <stdlib.h>
#include <gtk/gtk.h>
#include <libayatana-appindicator/app-indicator.h>
#include <stdio.h>
#include <unistd.h>

AppIndicator *tray_icon;
GtkWidget *menu;

/* Handle menu item actions */
void tray_icon_on_click(GtkMenuItem *item, gpointer user_data) {
    const char *action = (const char *)user_data;

    if (g_strcmp0(action, "save") == 0) {
        system("save2flash &");
    } else if (g_strcmp0(action, "settings") == 0) {
        system("eventmanager pm13 &");
    }
}

/* Create AppIndicator tray icon */
static AppIndicator *create_tray_icon() {
    menu = gtk_menu_new();

    // Menu item: Save Session Now
    GtkWidget *save_item = gtk_menu_item_new_with_label("Save Session Now");
    g_signal_connect(save_item, "activate", G_CALLBACK(tray_icon_on_click), (gpointer)"save");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), save_item);

    // Menu item: Open Pupmode 13 Settings
    GtkWidget *settings_item = gtk_menu_item_new_with_label("Open Pupmode 13 Settings");
    g_signal_connect(settings_item, "activate", G_CALLBACK(tray_icon_on_click), (gpointer)"settings");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), settings_item);

    gtk_widget_show_all(menu);

    tray_icon = app_indicator_new(
        "pupmode13-indicator",
        "floppy",  // XDG icon name
        APP_INDICATOR_CATEGORY_APPLICATION_STATUS
    );

    app_indicator_set_status(tray_icon, APP_INDICATOR_STATUS_ACTIVE);
    app_indicator_set_menu(tray_icon, GTK_MENU(menu));
    app_indicator_set_title(tray_icon, "Pupmode 13");

    return tray_icon;
}

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);
    create_tray_icon();
    gtk_main();
    return 0;
}
