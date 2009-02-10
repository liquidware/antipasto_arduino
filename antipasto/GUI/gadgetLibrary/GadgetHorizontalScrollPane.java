package antipasto.GUI.gadgetLibrary;

import java.awt.Color;
import java.awt.FlowLayout;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.ScrollPaneLayout;

import antipasto.Interfaces.IModule;


public class GadgetHorizontalScrollPane extends JPanel{
	JScrollPane pane;
	public GadgetHorizontalScrollPane(){
		super();

		Color white = Color.white;
		this.setBackground(white);

		FlowLayout flow = new FlowLayout();
		this.setLayout(flow);
	}
	
	public void add(IModule gadget){
		GadgetViewer viewer = new GadgetViewer(gadget);
		this.add(viewer);
	}
	
	public static void main(String args[]){
		JFrame frame = new JFrame("Testing Window");
		GadgetHorizontalScrollPane pane = new GadgetHorizontalScrollPane();
		frame.add(pane);
		pane.setVisible(true);
		frame.setVisible(true);
	}
}
