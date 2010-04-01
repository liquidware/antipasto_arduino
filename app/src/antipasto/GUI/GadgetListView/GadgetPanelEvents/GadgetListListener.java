package antipasto.GUI.GadgetListView.GadgetPanelEvents;


import javax.swing.event.ListSelectionListener;
import javax.swing.event.ListSelectionEvent;

import antipasto.GUI.GadgetListView.GadgetList;
import antipasto.Interfaces.IModule;

public class GadgetListListener implements ListSelectionListener {
    public void valueChanged(ListSelectionEvent e) {
        GadgetList list = (GadgetList) e.getSource();
        if(list.getSelectedValue() instanceof IModule){
            System.out.println("Congratualtions its worked");
        }
    }
}
