package model;

import java.sql.Timestamp;

public class StockHistory {
    private int id;
    private int drugId;
    private int changeAmount;
    private String info;
    private Timestamp createTime;

    public StockHistory() {}

    public StockHistory(int drugId, int changeAmount, String info) {
        this.drugId = drugId;
        this.changeAmount = changeAmount;
        this.info = info;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getDrugId() {
        return drugId;
    }

    public void setDrugId(int drugId) {
        this.drugId = drugId;
    }

    public int getChangeAmount() {
        return changeAmount;
    }

    public void setChangeAmount(int changeAmount) {
        this.changeAmount = changeAmount;
    }

    public String getInfo() {
        return info;
    }

    public void setInfo(String info) {
        this.info = info;
    }

    public Timestamp getCreateTime() {
        return createTime;
    }

    public void setCreateTime(Timestamp createTime) {
        this.createTime = createTime;
    }

    @Override
    public String toString() {
        return "StockHistory{" +
                "id=" + id +
                ", drugId=" + drugId +
                ", changeAmount=" + changeAmount +
                ", info='" + info + '\'' +
                '}';
    }
}
