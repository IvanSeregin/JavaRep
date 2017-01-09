/**
 * Created by NortT on 01.01.2017.
 */
public class Spaceport
{
    MissionControlCentre missionControlCentre;
    //

    public MissionControlCentre getMissionControlCentre()
    {
        return missionControlCentre;
    }

    public void setMissionControlCentre(MissionControlCentre missionControlCentre)
    {
        this.missionControlCentre = missionControlCentre;
    }



    public void initWithLunchable(Lunchable lunchable)
    {
        missionControlCentre.setLunchable(lunchable);
    }

    public void startLunchable()
    {
        missionControlCentre.lunch();
    }

    public void landLandable()
    {
        missionControlCentre.landLandable();
    }
}
