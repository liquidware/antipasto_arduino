package antipasto.Plugins.Manager;

import java.awt.*;
import javax.swing.*;
import processing.app.Base;

public class PluginListRenderer extends DefaultListCellRenderer {

    public Component getListCellRendererComponent(JList list, Object value,
            int index, boolean isSelected, boolean cellHasFocus) {

        JLabel label = (JLabel) super.getListCellRendererComponent(list, value,
                index, isSelected, cellHasFocus);

        if (index % 2 == 0) {
            label.setBackground(new Color(230, 230, 255));
        }

        label.setIcon(new ImageIcon(Base.getImage("js.png", this)));

        return label;
    }
}
 