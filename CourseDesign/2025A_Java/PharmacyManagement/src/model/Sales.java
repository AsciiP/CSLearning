package model;

import java.math.BigDecimal;
import java.sql.Timestamp;

public class Sales {
    private int id;
    private int drugId;
    private int quantity;
    private BigDecimal totalPrice;
    private Timestamp saleTime;
    private Integer userId;

    public Sales() {}

    public Sales(int drugId, int quantity, BigDecimal totalPrice, Integer userId) {
        this.drugId = drugId;
        this.quantity = quantity;
        this.totalPrice = totalPrice;
        this.userId = userId;
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

    public int getQuantity() {
        return quantity;
    }

    public void setQuantity(int quantity) {
        this.quantity = quantity;
    }

    public BigDecimal getTotalPrice() {
        return totalPrice;
    }

    public void setTotalPrice(BigDecimal totalPrice) {
        this.totalPrice = totalPrice;
    }

    public Timestamp getSaleTime() {
        return saleTime;
    }

    public void setSaleTime(Timestamp saleTime) {
        this.saleTime = saleTime;
    }

    public Integer getUserId() {
        return userId;
    }

    public void setUserId(Integer userId) {
        this.userId = userId;
    }
}
