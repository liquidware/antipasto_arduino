package antipasto.GUI.GadgetListView;

import javax.swing.*;


import java.awt.event.MouseListener;
import java.awt.event.MouseEvent;
import java.awt.*;


public class AddGadgetMenu implements MouseListener {
    public JPopupMenu menu;
    private JButton _button;

    public AddGadgetMenu(JPopupMenu menu, JButton button){
        this.menu = menu;
        _button = button;
        this.buildMenu();
    }

    public void mouseClicked(MouseEvent e) {
        if(menu.isVisible())
        {
            menu.setVisible(false);
        }
        int x = e.getX();
        int y = e.getY();

        int yMenuLocation = y;
        int xMenuLocation = x;

        menu.setLocation(xMenuLocation, yMenuLocation);
        menu.setVisible(true);
    }

    public void mousePressed(MouseEvent e) {

    }

    public void mouseReleased(MouseEvent e) {

    }

    public void mouseEntered(MouseEvent e) {

    }

    public void mouseExited(MouseEvent e) {

    }

    private void buildMenu()
    {
        menu.add(this.createMenuItem("Load from file"));
        menu.add(this.createMenuItem("Launch Gadget Library"));
    }

    private JMenuItem createMenuItem(String label){
        JMenuItem item = new JMenuItem(label);
        item.addMouseListener(new MenuMouseListener(item));
        return item;
    }

    private class MenuMouseListener implements MouseListener{

        JMenuItem _item;

        public MenuMouseListener(JMenuItem item){
            _item = item;
        }

        public void mouseClicked(MouseEvent e) {
            _item.getParent().setVisible(false);
            
        }

        public void mousePressed(MouseEvent e) {
        }

        public void mouseReleased(MouseEvent e) {
        }

        public void mouseEntered(MouseEvent e) {
            _item.setBackground(Color.blue);
        }

        public void mouseExited(MouseEvent e) {
            _item.setBackground(_item.getParent().getBackground());
        }
    }
}


