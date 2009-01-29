package antipasto.GUI.GadgetListView.GadgetPanelEvents;

import java.util.EventListener;


public interface IActiveSketchChangingListener extends EventListener {
    public void  onActiveSketchChanged(SketchChangingObject obj);
}
