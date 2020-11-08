#include "gtktemplate.h"

void packboxes();
void backup();
void cancel();

// compile with gcc -Wall -g Tback.c -o Tback `pkg-config --cflags --libs gtk+-3.0`
struct widgets
{
	GtkWidget *window;
	GtkWidget *vbox; //widget for the vertical box
	GtkWidget *hbox;
	GtkWidget *buttonbox;
	GtkWidget *entries[2];
	GtkWidget *entrygrid;
	GtkWidget *grid;
	GtkWidget *label;
}gwidget;

struct variables
{
	const char *entries[2];
}location;

gchar *labeltext[] = {"Source Location\n", "Destination Location\n"};
size_t label_len = arraysize(labeltext);

int main(int argc, char *argv [])
{
gchar *buttonlabels[] = {"Backup", "Cancel"};
void *buttoncallbacks[] = {backup, cancel};

gtk_init(&argc, &argv); //starting gtk 

gwidget.window = createwindow("Tback", GTK_WIN_POS_CENTER);

gwidget.buttonbox = createsinglesizegrid(buttonlabels, buttoncallbacks, 1, 2);
gwidget.label = createlabels(labeltext, label_len);
gwidget.entrygrid = gtk_grid_new();
create_entries(label_len, gwidget.entries, gwidget.entrygrid);
packboxes();

show_and_destroy(gwidget.window);
}

void packboxes()
{
	gwidget.vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1); //creates a vbox without autosizing 
	gwidget.hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); //creates a vbox without autosizing 
	gtk_container_add(GTK_CONTAINER(gwidget.window), gwidget.vbox); //adds the vbox to the window 
	gtk_box_pack_start(GTK_BOX(gwidget.vbox),  gwidget.hbox, FALSE, FALSE, 0); //packs the display into the vbox
	gtk_box_pack_start(GTK_BOX(gwidget.hbox),  gwidget.label, FALSE, FALSE, 0); //packs the display into the vbox
	gtk_box_pack_start(GTK_BOX(gwidget.hbox),  gwidget.entrygrid, FALSE, FALSE, 0); //packs the display into the vbox
	gtk_box_pack_start(GTK_BOX(gwidget.vbox),  gwidget.buttonbox, FALSE, FALSE, 0); //packs the display into the vbox
}

void backup()
{
	get_entry_text(gwidget.entries, location.entries, label_len);
	char outputbuf[BUFSIZ];
	snprintf(outputbuf, sizeof(outputbuf), "cp -r %s %s", location.entries[0], location.entries[1]);
	system(outputbuf);
}

void cancel()
{
exit(1);
}
