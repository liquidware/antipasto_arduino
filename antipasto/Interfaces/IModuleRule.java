package antipasto.Interfaces;

public interface IModuleRule {
    public String getGadgetName();
    public void setGadgetName(String gadgetName);
    public boolean isHasMessage();
    public void setHasMessage(boolean hasMessage);
    public boolean isNeedsContinueButton();
    public void setNeedsContinueButton(boolean needsContinueButton);
    public boolean isNeedsRetryButton();
    public void setNeedsRetryButton(boolean needsRetryButton);
    public IModule getGadget();
    public void setGadget(IModule gadget);
    public String getMessage();
    public void setMessage(String message);
}
