package processing.app;

import java.awt.*;
import java.util.StringTokenizer;

import javax.swing.*;
import java.awt.event.*;

public class PreferencesCellRenderer extends DefaultListCellRenderer implements ActionListener  {

        /* This is the only method defined by ListCellRenderer.  We just
     * reconfigure the JLabel each time we're called.
     */
    public Component getListCellRendererComponent(
        JList list,
	Object value,   // value to display
	int index,      // cell index
	boolean isSelected,    // is the cell selected
	boolean chf)    // the list and the cell have the focus
    {
        /* The DefaultListCellRenderer class will take care of
         * the JLabels text property, it's foreground and background
         * colors, and so on.
         */
        JLabel label = (JLabel)super.getListCellRendererComponent(list, value, index, isSelected, chf);
        
        JPanel cell = new JPanel();
        cell.setLayout(new FlowLayout(FlowLayout.LEFT));

        String leftS = "";
        String rightS = "";

        try {
            StringTokenizer st = new StringTokenizer((String)value,"=");
            leftS = st.nextToken();
            rightS = st.nextToken();
        } catch (Exception ex) {
        }

        JLabel left = new JLabel(leftS);
        JTextField right = new JTextField(rightS); 
        right.setEditable(true);
        right.setEnabled(true);
        right.addActionListener(this);
         
        cell.add(left);
        cell.add(right);

        return cell;
    }

    public void actionPerformed(ActionEvent evt) {
        ;
    }

}
