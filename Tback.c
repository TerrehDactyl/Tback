#include <gtk/gtk.h>
#define arraysize(x)  (sizeof(x) / sizeof((x)[0]))

void add_context(const gchar *style, GtkWidget *widget);
void packboxes();
void createwindow(const gchar *style);
void create_entries();
void set_spacing(GtkWidget *widget, int colspace, int rowspace);
void createlabels(GtkWidget *label, gchar *labeltext[], int label_len, const gchar *style);
void createsinglesizegrid(GtkWidget *grid, gchar *labels[], void *callback[], int rows, int columns, const gchar *style[], int stylenumber);
void button_connect_callback(GtkWidget *button, void *button_callback);
void backup();

// compile with gcc -Wall -g backup.c -o backup `pkg-config --cflags --libs gtk+-3.0`
struct widgets
{
	GtkWidget *window;
	GtkWidget *vbox; //widget for the vertical box
	GtkWidget *hbox;
	GtkWidget *buttonbox;
	GtkWidget *buttonhbox;
	GtkWidget *SourceEntry;
	GtkWidget *DestinationEntry;
	GtkWidget *entrygrid;
	GtkWidget *grid;
	GtkWidget *label;
}gwidget;

struct variables
{
	const char *source;
	const char *destination;
}location;

int main(int argc, char *argv [])
{
const gchar *style[] = {"window_style", "label_style", "button_style"};
gchar *labeltext[] = {"Source Location\n", "Destination Location\n"};
gchar *buttonlabels[] = {"Backup", "Cancel"};
void *buttoncallbacks[] = {backup};
size_t label_len = arraysize(labeltext);

gtk_init(&argc, &argv); //starting gtk 

createwindow(style[0]);

gwidget.buttonbox = gtk_grid_new(); 
createlabels(gwidget.label, labeltext, label_len, style[1]);
createsinglesizegrid(gwidget.buttonbox, buttonlabels, buttoncallbacks, 1, 2, style, 2);
set_spacing(gwidget.buttonbox, 4, 4);
create_entries();
packboxes();

g_signal_connect(G_OBJECT(gwidget.window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

gtk_widget_show_all(gwidget.window); //shows all widgets 

gtk_main();//gtk main, this is the main loop of GTK
}

void add_context(const gchar *style, GtkWidget *widget)
{
	GtkCssProvider* Provider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(Provider), "Styles.css", NULL);
	GtkStyleContext *context = gtk_widget_get_style_context(widget);
	gtk_style_context_add_class(context, style);
	gtk_style_context_add_provider (context,GTK_STYLE_PROVIDER(Provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

void createwindow(const gchar *style)
{
	gwidget.window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //creates toplevel window
	gtk_window_set_title(GTK_WINDOW(gwidget.window), "Tback"); //sets a window title 
	gtk_window_set_position(GTK_WINDOW(gwidget.window), GTK_WIN_POS_CENTER); //opens the window in the center of the screen
	gtk_container_set_border_width(GTK_CONTAINER(gwidget.window), 5); //sets the border size of the window
	add_context(style, gwidget.window);
}

void packboxes()
{
	gwidget.vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1); //creates a vbox without autosizing 
	gwidget.hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); //creates a vbox without autosizing 
	gwidget.buttonhbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); //creates a vbox without autosizing 
	gtk_container_add(GTK_CONTAINER(gwidget.window), gwidget.vbox); //adds the vbox to the window 
	gtk_box_pack_start(GTK_BOX(gwidget.vbox),  gwidget.hbox, FALSE, FALSE, 0); //packs the display into the vbox
	gtk_box_pack_start(GTK_BOX(gwidget.vbox),  gwidget.buttonbox, FALSE, FALSE, 0); //packs the display into the vbox
	gtk_box_pack_start(GTK_BOX(gwidget.hbox),  gwidget.grid, FALSE, FALSE, 0); //packs the display into the vbox
	gtk_box_pack_start(GTK_BOX(gwidget.hbox),  gwidget.entrygrid, FALSE, FALSE, 0); //packs the display into the vbox
}

void create_entries()
{
	gwidget.entrygrid = gtk_grid_new();
	gwidget.SourceEntry = gtk_entry_new();
	gwidget.DestinationEntry = gtk_entry_new();
	gtk_grid_attach(GTK_GRID(gwidget.entrygrid), gwidget.SourceEntry, 0, 0, 1, 1); //sets the defaults for creating each table button
	gtk_grid_attach(GTK_GRID(gwidget.entrygrid), gwidget.DestinationEntry, 0,1,1,1);
}

void createlabels(GtkWidget *label, gchar *labeltext[], int label_len, const gchar *style)
{
	gwidget.grid = gtk_grid_new();

for (int i=0; i<label_len; i++)
{
	for(int j = 0; j<1; j++)
	{	
		label = gtk_label_new(labeltext[i]);
		add_context(style, label);
		gtk_grid_attach(GTK_GRID(gwidget.grid), label, j, i, 1, 1); //sets the defaults for creating each table button
	}
}
}

void createsinglesizegrid(GtkWidget *grid, gchar *labels[], void *callback[], int rows, int columns, const gchar *style[], int stylenumber)
{
int pos = 0;
GtkWidget *button; //widget for the buttons 
	for (int i=0; i < rows; i++) //for loop for the rows
	{
		for (int j=0; j < columns; j++) //for loop for the columns
		{
		button = gtk_button_new_with_label(labels[pos]); //sets each button label to the respective button 
		button_connect_callback(button, callback[pos]); //attaches the button to the respective callback
		gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1); //sets the defaults for creating each table button
		gtk_widget_set_size_request(button, 70, 30); //sets the size of the buttons
		add_context(style[stylenumber], button);
		pos++; //changes the position 
		}
	}
}

void button_connect_callback(GtkWidget *button, void *button_callback)
{
	g_signal_connect(button, "clicked", G_CALLBACK(button_callback), NULL);
}

void set_spacing(GtkWidget *widget, int colspace, int rowspace)
{
	gtk_grid_set_column_spacing(GTK_GRID(widget), colspace);
	gtk_grid_set_row_spacing(GTK_GRID(widget), rowspace);
}

void backup()
{
	location.source = gtk_entry_get_text (GTK_ENTRY (gwidget.SourceEntry));
	location.destination = gtk_entry_get_text(GTK_ENTRY (gwidget.DestinationEntry));
	char outputbuf[BUFSIZ];
	snprintf(outputbuf, sizeof(outputbuf), "cp -r %s %s", location.source, location.destination);
	system(outputbuf);
	g_print("%s", outputbuf);
}

void cancel()
{
exit(1);
}