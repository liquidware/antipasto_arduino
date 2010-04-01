package antipasto.Interfaces;

/**
 * Created by IntelliJ IDEA.
 * User: Omar
 */
public interface IVersionable {
    public int getMajorNumber();
    public int getMinorNumber();
    public int getRevisionNumber();

    public void setMajorNumber(int value);
    public void setMinorNumber(int value);
    public void setRevisionNumber(int value);
}
