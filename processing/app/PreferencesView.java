package processing.app;

import java.awt.*;
import java.awt.event.*;

import javax.swing.*;
import javax.swing.event.*;

import java.util.*;
import java.util.Hashtable;

public class PreferencesView extends JList implements ListSelectionListener {
    final Preferences pref;

    public PreferencesView(Hashtable table, Preferences preferences) {
        super();
        pref = preferences;
        this.setCellRenderer(new PreferencesCellRenderer());
        PreferencesModel preferencesModel = new PreferencesModel(table);
        this.setModel(preferencesModel);
        addListSelectionListener(this);

        /* This is a hack and only works for checkboxes */
        MouseListener mouseListener = new MouseAdapter() {
          public void mouseClicked(MouseEvent mouseEvent) {
            JList theList = (JList) mouseEvent.getSource();
            int index = theList.locationToIndex(mouseEvent.getPoint());
            if (index >= 0) {
                Object o = theList.getModel().getElementAt(index);
                System.out.println("Clicked on: " + o.toString());
                try {
                    StringTokenizer st = new StringTokenizer(o.toString(),"=");
                    String leftS = st.nextToken();
                    String rightS = st.nextToken();
                    if (rightS.equals("true")) {
                       rightS = "false";     
                    } else if( rightS.equals("false") ) {
                       rightS = "true";
                    }
                    pref.set(leftS,rightS);
                    theList.repaint();
                } catch (Exception ex) {
                }
              }
          }
        };

        this.addMouseListener(mouseListener);

    }


    // ListSelectionListener implementation
    public void valueChanged (ListSelectionEvent lse) {
        //System.out.println(lse);

        if (!lse.getValueIsAdjusting()) {
            JList list = (JList)lse.getSource();

            // Get all selected items
            Object[] selected = list.getSelectedValues();

            // Iterate all selected items
            for (int i=0; i<selected.length; i++) {
                Object sel = selected[i];
            }
        }
    }

}
