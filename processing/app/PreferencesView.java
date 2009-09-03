package processing.app;

import java.awt.*;
import java.util.Hashtable;

import javax.swing.*;

public class PreferencesView extends JList {

    public PreferencesView(Hashtable table) {

        this.setCellRenderer(new PreferencesCellRenderer());
        PreferencesModel preferencesModel = new PreferencesModel(table);
        this.setModel(preferencesModel);
    }

}
