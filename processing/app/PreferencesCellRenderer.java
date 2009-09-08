package processing.app;

import java.awt.*;
import java.util.StringTokenizer;

import javax.swing.*;
import java.awt.event.*;

public class PreferencesCellRenderer extends JComponent implements ListCellRenderer {
    String leftS = "";
    String rightS = "";
    JPanel cell = new JPanel();
    JCheckBox checkBox = new JCheckBox();
    JTextField textField = new JTextField();
    DefaultListCellRenderer defaultComp = new DefaultListCellRenderer();
    boolean checkBoxValue = false;

    /* This is the only method defined by ListCellRenderer.  We just
     * reconfigure the JLabel each time we're called.
     */
    public Component getListCellRendererComponent(
        JList list,
	Object value,   // value to display
	int index,      // cell index
	boolean isSelected,    // is the cell selected
	boolean cellHasFocus)    // the list and the cell have the focus
    {
        /* The DefaultListCellRenderer class will take care of
         * the JLabels text property, it's foreground and background
         * colors, and so on.
         */
        defaultComp.getListCellRendererComponent (list, value, index,
                                                      isSelected, cellHasFocus);
        
        JPanel cell = new JPanel();
        cell.setLayout(new FlowLayout(FlowLayout.LEFT));

        try {
            StringTokenizer st = new StringTokenizer((String)value,"=");
            leftS = st.nextToken();
            rightS = st.nextToken();
        } catch (Exception ex) {
        }

        JLabel left = new JLabel(leftS);
        cell.add(left);

        /* An ugly hack to determine what type of UI control to show. */
        if (rightS.toLowerCase().equals("true") || rightS.toLowerCase().equals("false")) {
            /* A checkbox */
            checkBox = new JCheckBox(rightS);
            if (rightS.toLowerCase().equals("true")) {
                checkBox.setSelected(true);
            } else {
                checkBox.setSelected(false);
            }
            cell.add(checkBox);
        } else {
            /* A text Field */
            textField = new JTextField(rightS); 
            textField.setEditable(true);
            textField.setEnabled(true);
            cell.add(textField);
        }
        
        return cell;
    }

}
