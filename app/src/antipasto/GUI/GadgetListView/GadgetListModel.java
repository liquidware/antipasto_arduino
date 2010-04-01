package antipasto.GUI.GadgetListView;


import javax.swing.*;
import javax.swing.event.ListDataListener;
import javax.swing.event.EventListenerList;
import javax.swing.event.ListDataEvent;

import antipasto.GUI.GadgetListView.GadgetPanelEvents.IGadgetCollectionChangedListener;


public class GadgetListModel implements ListModel, IGadgetCollectionChangedListener {
    private GadgetCollection _gadgets;
    private EventListenerList listDataListener = new EventListenerList();

    public GadgetListModel(GadgetCollection gadgets){
        this.loadGadgets(gadgets);
    }

    public void loadGadgets(GadgetCollection gadgets){
        _gadgets = gadgets;
    }

    public int getSize() {
        return _gadgets.getLength();
    }

    public Object getElementAt(int index) {
        return _gadgets.getItem(index);
    }

    public void addListDataListener(ListDataListener l) {
        listDataListener.add(ListDataListener.class, l);
    }

    public void removeListDataListener(ListDataListener l) {
        listDataListener.remove(ListDataListener.class, l);
    }

    private void onListDataChanged(){
        Object[] listeners = listDataListener.getListenerList();
                    // Each listener occupies two elements - the first is the listener class
                    // and the second is the listener instance
                    for (int i=0; i<listeners.length; i+=2) {
                        if (listeners[i]== ListDataListener.class) {
                            ((ListDataListener)listeners[i+1]).contentsChanged(new ListDataEvent(this, 0,0,0));
                        }
                    }
    }

    public void OnGadgetCollectionChanged(GadgetCollection gc) {
        this.onListDataChanged();
    }
}
