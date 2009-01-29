package antipasto.GUI.GadgetListView;


import javax.swing.event.EventListenerList;

import antipasto.GUI.GadgetListView.GadgetPanelEvents.IGadgetCollectionChangedListener;
import antipasto.Interfaces.IGadget;
import antipasto.Interfaces.IModule;

import java.util.Arrays;

public class GadgetCollection {
    IModule[] _gadgets;
    IGadget _sketchBook;

    EventListenerList changeListenerList = new EventListenerList();

    public GadgetCollection(IGadget sketchBook){
        _gadgets = sketchBook.getModules();
        _sketchBook = sketchBook;
        onGadgetCollectionChanged();
    }

    public void AddGadget(IModule gadget){
        IModule[] gadgetArr = new IModule[_gadgets.length + 1];
        Arrays.fill(_gadgets, gadgetArr);
        gadgetArr[_gadgets.length] = gadget;
        _gadgets = gadgetArr;
        onGadgetCollectionChanged();
    }

    public void RemoveGadget(IModule gadget){
        int gadIndex = -1;
        for(int i = 0; i < _gadgets.length; i++)
        {
            if(gadget.getName().equalsIgnoreCase(_gadgets[i].getName()))
            {
                gadIndex = i;
                break;
            }
        }

        IModule[] gadArr = new IModule[_gadgets.length - 1];

        int x = 0;
        for(int i = 0; i < _gadgets.length; i++)
        {
            if(i != gadIndex)
            {
                gadArr[x] = _gadgets[i];
                x++;
            }
        }
        onGadgetCollectionChanged();
    }

    public int getLength(){
        return _gadgets.length;
    }

    public IModule getItem(int index){
        return _gadgets[index];
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
