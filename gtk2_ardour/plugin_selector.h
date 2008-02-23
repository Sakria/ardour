/*
    Copyright (C) 2000 Paul Davis 

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#ifndef __ardour_plugin_selector_h__
#define __ardour_plugin_selector_h__

#include <gtkmm/dialog.h>
#include <gtkmm/notebook.h>
#include <gtkmm/treeview.h>
#include <gtkmm2ext/selector.h>

#include <ardour/plugin.h>

namespace ARDOUR {
	class Session;
	class PluginManager;
}

class PluginSelector : public ArdourDialog
{
  public:
	PluginSelector (ARDOUR::PluginManager *);
	sigc::signal<void,boost::shared_ptr<ARDOUR::Plugin> > PluginCreated;

	int run (); // XXX should we try not to overload the non-virtual Gtk::Dialog::run() ?

	void set_session (ARDOUR::Session*);
	void on_show ();

  private:
	ARDOUR::Session* session;
	Gtk::ScrolledWindow scroller;  // Available plugins
	Gtk::ScrolledWindow ascroller;  // Added plugins

	Gtk::ComboBoxText filter_mode;
	Gtk::Entry filter_entry;
	Gtk::Button filter_button;

	void filter_button_clicked ();
	void filter_entry_changed ();
	void filter_mode_changed ();
	
	struct PluginColumns : public Gtk::TreeModel::ColumnRecord {
		PluginColumns () {
			add (name);
			add (type_name);
			add (category);
			add (creator);
			add (ins);
			add (outs);
			add (plugin);
		}
		Gtk::TreeModelColumn<std::string> name;
		Gtk::TreeModelColumn<std::string> type_name;
		Gtk::TreeModelColumn<std::string> category;
		Gtk::TreeModelColumn<std::string> creator;
		Gtk::TreeModelColumn<std::string> ins;
		Gtk::TreeModelColumn<std::string> outs;
		Gtk::TreeModelColumn<ARDOUR::PluginInfoPtr> plugin;
	};
	PluginColumns plugin_columns;
	Glib::RefPtr<Gtk::ListStore> plugin_model;
	Gtk::TreeView plugin_display;
	Gtk::Button* btn_add;
	Gtk::Button* btn_remove;

	struct AddedColumns : public Gtk::TreeModel::ColumnRecord {
		AddedColumns () {
			add (text);
			add (plugin);
		}
		Gtk::TreeModelColumn<std::string> text;
		Gtk::TreeModelColumn<ARDOUR::PluginInfoPtr> plugin;
	};
	AddedColumns acols;
	Glib::RefPtr<Gtk::ListStore> amodel;
	Gtk::TreeView added_list;

	void refill ();
	void refiller (const ARDOUR::PluginInfoList& plugs, const::std::string& filterstr, const char* type);
	void ladspa_refiller (const std::string&);
	void lv2_refiller (const std::string&);
	void vst_refiller (const std::string&);
	void au_refiller (const std::string&);

	ARDOUR::PluginManager *manager;

	void row_clicked(GdkEventButton *);
	void btn_add_clicked();
	void btn_remove_clicked();
	void btn_update_clicked();
	void added_list_selection_changed();
	void display_selection_changed();
	void btn_apply_clicked();
	void use_plugin (ARDOUR::PluginInfoPtr);
	void cleanup ();
	bool show_this_plugin (const ARDOUR::PluginInfoPtr&, const std::string&);
	void setup_filter_string (std::string&);
};

#endif // __ardour_plugin_selector_h__

