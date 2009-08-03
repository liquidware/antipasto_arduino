package antipasto.Plugins;

import java.awt.*;
import javax.swing.*;
import javax.swing.event.*;

import java.util.*;
import java.awt.event.*;
import org.java.plugin.registry.PluginDescriptor;

public class PluginListModel extends AbstractListModel {

    private ArrayList<Map> values = new ArrayList<Map>();

    public Object getElementAt(int index) {
        Map m = values.get(index);
        return m.get("value");
    }

    public PluginDescriptor getDescriptorAt(int index) {
        Map m = values.get(index);
        return (PluginDescriptor) m.get("id");
    }

    public int getSize() {
        return values.size();
    }

    public void addElement(PluginDescriptor id, String value) {
        Map m = new HashMap();
        m.put("id", id);
        m.put("value", value);
        values.add(m);

        update(this.getSize() - 1, this.getSize());

    }

    public void removeElement(int index) {
        values.remove(index);
        update(0, this.getSize());
    }

    public void update(int von, int bis) {
        fireIntervalAdded(this, von, bis);
    }
} 