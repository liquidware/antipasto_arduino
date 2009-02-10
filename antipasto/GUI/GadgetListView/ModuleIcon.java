package antipasto.GUI.GadgetListView;

import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.event.EventListenerList;

import antipasto.Interfaces.IModule;

public class ModuleIcon extends JLabel {
	IModule internalModule;
	EventListenerList listeners = new EventListenerList();
	public ModuleIcon(IModule module){
		super(new ImageIcon(module.getImage()));
		internalModule = module;
		this.addMouseListener(new MouseListener(){
			public void mouseClicked(MouseEvent arg0) {
				fireSelectedEvent();
				System.out.println("Firing selection event");
			}
			public void mouseEntered(MouseEvent arg0) {
			}
			public void mouseExited(MouseEvent arg0) {
			}
			public void mousePressed(MouseEvent arg0) {
			}
			public void mouseReleased(MouseEvent arg0) {
			}
		});
	}
	
	public IModule getModule(){
		return this.internalModule;
	}
	
	public void addSelectionListener(ISelectedItemListener item){
		listeners.add(ISelectedItemListener.class, item);
	}
	
	private void fireSelectedEvent(){
		Object[] listeners = this.listeners.getListenerList();
	     // loop through each listener and pass on the event if needed
	     int numListeners = listeners.length;
	     for (int i = 0; i<numListeners; i+=2) 
	     {
	          if (listeners[i]==ISelectedItemListener.class) 
	          {
	               // pass the event to the listeners event dispatch method
	                ((ISelectedItemListener)listeners[i+1]).onSelected(this);
	          }            
	     }

	}
	
	
}
