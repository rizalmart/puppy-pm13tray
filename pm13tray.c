#include <stdlib.h>
#include <gtk/gtk.h>
#include <libayatana-appindicator/app-indicator.h>
#include <stdio.h>
#include <unistd.h>

AppIndicator *tray_icon;
GtkWidget *menu;

/* XPM icon */
static char *floppy_xpm[] = {
"28 28 62 1",
" 	c None",
"A	c #2B2B2B",
"B	c #424242",
"C	c #454545",
"D	c #434B43",
"E	c #404040",
"F	c #202020",
"G	c #787878",
"H	c #6C6C6C",
"I	c #389F40",
"J	c #33A33C",
"K	c #767676",
"L	c #3D3D3D",
"M	c #434343",
"N	c #676767",
"O	c #383838",
"P	c #4A4A4A",
"Q	c #309738",
"R	c #2C9C35",
"S	c #555555",
"T	c #6B6B6B",
"U	c #686868",
"V	c #414141",
"W	c #4E4E4E",
"X	c #AAB9AB",
"Y	c #B2C0B3",
"Z	c #E2E2E2",
"a	c #EEEEEE",
"b	c #BFBFBF",
"c	c #BDBDBD",
"d	c #939393",
"e	c #989898",
"f	c #565656",
"g	c #595959",
"h	c #585858",
"i	c #A3A3A3",
"j	c #ABABAB",
"k	c #858585",
"l	c #7B7B7B",
"m	c #9F9F9F",
"n	c #9D9D9D",
"o	c #575757",
"p	c #5A5A5A",
"q	c #BBBBBB",
"r	c #969696",
"s	c #515151",
"t	c #535353",
"u	c #636363",
"v	c #909090",
"w	c #525252",
"x	c #656565",
"y	c #646464",
"z	c #393939",
"0	c #626262",
"1	c #5E5E5E",
"2	c #ADADAD",
"3	c #848484",
"4	c #000000",
"5	c #3A3A3A",
"6	c #6D6D6D",
"7	c #4D4D4D",
"8	c #2D2D2D",
"                            ",
"                            ",
"   BCCDDDDDDDDDDDDDDDDCCE   ",
"  BGHHIJJJJJJJJJJJJJJIHHKL  ",
"  MNOPQRRRRRRRRRRRRRRQSSTM  ",
"  MUVWXYYYYYYYYYYYYYYXSSTM  ",
"  MTSSZaaaaaaaaaaaaaaZSSTM  ",
"  MTSSZbccccccccccccbZSSTM  ",
"  MTSSZaaaaaaaaaaaaaaZSSTM  ",
"  MTSSZaaaaaaaaaaaaaaZSSTM  ",
"  MTSSZbccccccccccccbZSSTM  ",
"  MTSSZaaaaaaaaaaaaaaZSSTM  ",
"  MTSSZaaaaaaaaaaaaaaZSSTM  ",
"  MTSSZbccccccccccccbZSSTM  ",
"  MTSSZaaaaaaaaaaaaaaZSSTM  ",
"  MTSSdeeeeeeeeeeeeeedSSTM  ",
"  MTSSSSfggggggggggSSSSSTM  ",
"  MTSSShijklmjjjjjjnoSSSTM  ",
"  MTSSSpqrstuqqqqqqqpSSSTM  ",
"  MTSSSpqvwSxqqqqqqqpSSSTM  ",
"  MTSSSpqvwSxqqqqqqqpSSSTM  ",
"  MTSSSpqvwSxqqqqqqqpSSSTM  ",
"  MHSSSpqvwSyqqqqqqqpSSSTM  ",
"  z01SSpq2Nx3qqqqqqqpSSSTM  ",
"   5u6HuqqqqqqqqqqqquHHHGV  ",
"    zCC7pppppppppppp7CCCV   ",
"                            ",
"                            "
};

/* Write XPM icon to temp file */
char* write_temp_xpm_icon() {
    static char filename[] = "/tmp/iconXXXXXX.xpm";
    int fd = mkstemps(filename, 4); // ".xpm"
    if (fd == -1) {
        perror("mkstemps failed");
        return NULL;
    }

    FILE *fp = fdopen(fd, "w");
    if (!fp) {
        perror("fdopen failed");
        close(fd);
        return NULL;
    }

    for (int i = 0; floppy_xpm[i] != NULL; i++) {
        fprintf(fp, "%s\n", floppy_xpm[i]);
    }

    fclose(fp);
    return filename;
}

/* Handle menu item actions */
void tray_icon_on_click(GtkMenuItem *item, gpointer user_data) {
    const char *action = (const char *)user_data;

    if (g_strcmp0(action, "save") == 0) {
        system("eventmanager save-session &");
    } else if (g_strcmp0(action, "settings") == 0) {
        system("eventmanager pm13 &");
    }
}

/* Create AppIndicator tray icon */
static AppIndicator *create_tray_icon() {
    char *icon_path = write_temp_xpm_icon();
    if (!icon_path) {
        fprintf(stderr, "Failed to create temp icon file\n");
        exit(1);
    }

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
        icon_path,
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
