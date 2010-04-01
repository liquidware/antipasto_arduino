package antipasto.GUI.GadgetListView.AddGadgetActions;

import javax.swing.*;
import java.awt.event.ActionEvent;

public class AddGadget extends AbstractAction {

    public AddGadget(){
        super("Add Gadget");
    }

    public void actionPerformed(ActionEvent e) {
        System.out.println("Action performed");
    }
}
