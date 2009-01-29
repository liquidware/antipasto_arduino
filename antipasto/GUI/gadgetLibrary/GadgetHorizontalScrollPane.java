package antipasto.GUI.gadgetLibrary;

import java.awt.Color;
import java.awt.FlowLayout;

import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.ScrollPaneLayout;

import antipasto.Interfaces.IModule;


public class GadgetHorizontalScrollPane extends JPanel{
	JScrollPane pane;
	public GadgetHorizontalScrollPane(){
		super();

		Color black = Color.black;
		this.setBackground(black);

		FlowLayout flow = new FlowLayout();
		this.setLayout(flow);
	}
	
	public void add(IModule gadget){
		GadgetViewer viewer = new GadgetViewer(gadget);
		this.add(viewer);
	}
}
