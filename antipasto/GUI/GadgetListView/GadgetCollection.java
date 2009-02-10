package antipasto.GUI.GadgetListView;


import javax.swing.event.EventListenerList;

import antipasto.GUI.GadgetListView.GadgetPanelEvents.IGadgetCollectionChangedListener;
import antipasto.Interfaces.IGadget;
import antipasto.Interfaces.IModule;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class GadgetCollection {
    List _gadgets;
    IGadget _sketchBook;

    EventListenerList changeListenerList = new EventListenerList();

    public GadgetCollection(IGadget gadget){
        _gadgets = new ArrayList();
        
        for(int i = 0; i < gadget.getModules().length; i ++){
        	_gadgets.add(gadget.getModules()[i]);
        }
        _sketchBook = gadget;
        onGadgetCollectionChanged();
    }
    
    public GadgetCollection(){
    	_gadgets = new ArrayList();
    }

    public void AddGadget(IModule gadget){
    	this._gadgets.add(gadget);
    	onGadgetCollectionChanged();
    }

    public void RemoveGadget(IModule gadget){
        int gadIndex = -1;
        for(int i = 0; i < _gadgets.size(); i++)
        {
            if(gadget.getName().equalsIgnoreCase(((IModule) _gadgets.get(i)).getName()))
            {
                gadIndex = i;
                break;
            }
        }

        IModule[] gadArr = new IModule[_gadgets.size() - 1];

        int x = 0;
        for(int i = 0; i < _gadgets.size(); i++)
        {
            if(i != gadIndex)
            {
                gadArr[x] = (IModule) _gadgets.get(i);
                x++;
            }
        }
        onGadgetCollectionChanged();
    }

    public int getLength(){
        return _gadgets.size();
    }

    public IModule getItem(int index){
        return (IModule) _gadgets.get(index);
    }

    public void addGadgetCollectionEventListener(IGadgetCollectionChangedListener listener){
        this.changeListenerList.add(IGadgetCollectionChangedListener.class, listener);
    }

    public void removeGadgetCollectionEventListener(IGadgetCollectionChangedListener listener){
        this.changeListenerList.add(IGadgetCollectionChangedListener.class, listener);
    }

    private void onGadgetCollectionChanged(){
        Object[] listeners = changeListenerList.getListenerList();
                    // Each listener occupies two elements - the first is the listener class
                    // and the second is the listener instance
                    for (int i=0; i<listeners.length; i+=2) {
                        if (listeners[i]== IGadgetCollectionChangedListener.class) {
                            ((IGadgetCollectionChangedListener)listeners[i+1]).OnGadgetCollectionChanged(this);
                        }
                    }
    }
}
