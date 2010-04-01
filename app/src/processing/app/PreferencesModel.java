package processing.app;

import java.awt.*;
import javax.swing.*;
import java.util.ArrayList;
import java.util.*;
import java.io.*;

import javax.swing.ListModel;
import javax.swing.event.EventListenerList;
import javax.swing.event.ListDataEvent;
import javax.swing.event.ListDataListener;

public class PreferencesModel implements ListModel {

    private Hashtable table = new Hashtable();
    private Hashtable prefixes = new Hashtable();
    private EventListenerList listDataListener = new EventListenerList();

    public PreferencesModel(Hashtable table){
        this.table = table;
    }

    public void addListDataListener(ListDataListener l) {
        listDataListener.add(ListDataListener.class, l);
    }

    public Object getElementAt(int index) {
        int elementIndex=0;
        Enumeration e = table.keys();
        String value = "default";
        while(e.hasMoreElements() && (elementIndex <= index)) {
            value = (String)e.nextElement();
            elementIndex++;
        }
        return value + "=" + table.get(value);
    }

    public int getSize() {
        return table.size();
    }

    public void removeListDataListener(ListDataListener l) {
        listDataListener.remove(ListDataListener.class, l);
    }

    public void itemsChanged(){
        this.onListDataChanged();
    }

    public void remove(int index){
        table.remove(index);
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

    public void filesChanged() {
        this.onListDataChanged();
    }

}
