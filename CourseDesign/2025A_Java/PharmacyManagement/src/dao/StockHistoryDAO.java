package dao;

import model.StockHistory;
import util.DBUtil;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class StockHistoryDAO {
    
    public List<StockHistory> findByDrugId(int drugId) {
        List<StockHistory> list = new ArrayList<>();
        String sql = "SELECT id, drug_id, change_amount, info, create_time FROM stock_history WHERE drug_id = ? ORDER BY create_time DESC";
        try (Connection conn = DBUtil.getConnection();
             PreparedStatement ps = conn.prepareStatement(sql)) {
            ps.setInt(1, drugId);
            ResultSet rs = ps.executeQuery();
            while (rs.next()) {
                StockHistory sh = new StockHistory();
                sh.setId(rs.getInt("id"));
                sh.setDrugId(rs.getInt("drug_id"));
                sh.setChangeAmount(rs.getInt("change_amount"));
                sh.setInfo(rs.getString("info"));
                sh.setCreateTime(rs.getTimestamp("create_time"));
                list.add(sh);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return list;
    }

    public void insert(StockHistory history) {
        String sql = "INSERT INTO stock_history(drug_id, change_amount, info) VALUES(?, ?, ?)";
        try (Connection conn = DBUtil.getConnection();
             PreparedStatement ps = conn.prepareStatement(sql)) {
            ps.setInt(1, history.getDrugId());
            ps.setInt(2, history.getChangeAmount());
            ps.setString(3, history.getInfo());
            ps.executeUpdate();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
