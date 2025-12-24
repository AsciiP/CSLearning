package dao;

import model.Sales;
import util.DBUtil;

import java.sql.*;
import java.math.BigDecimal;
import java.util.HashMap;
import java.util.Map;

public class SalesDAO {
    public void insert(Sales sales) {
        String sql = "INSERT INTO sales (drug_id, quantity, total_price, user_id) VALUES (?, ?, ?, ?)";
        try (Connection conn = DBUtil.getConnection();
                PreparedStatement ps = conn.prepareStatement(sql)) {
            ps.setInt(1, sales.getDrugId());
            ps.setInt(2, sales.getQuantity());
            ps.setBigDecimal(3, sales.getTotalPrice());
            ps.setObject(4, sales.getUserId());
            ps.executeUpdate();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 获取用户业绩统计
     * 返回 Map<用户名, [销售笔数, 总销售额]>
     */
    public Map<String, Object[]> getUserPerformance() {
        Map<String, Object[]> result = new HashMap<>();
        String sql = "SELECT u.username, COUNT(s.id) as count, COALESCE(SUM(s.total_price), 0) as total " +
                "FROM user u " +
                "LEFT JOIN sales s ON u.id = s.user_id " +
                "WHERE u.role = 'user' " +
                "GROUP BY u.id, u.username " +
                "ORDER BY total DESC";

        try (Connection conn = DBUtil.getConnection();
                PreparedStatement ps = conn.prepareStatement(sql);
                ResultSet rs = ps.executeQuery()) {

            while (rs.next()) {
                String username = rs.getString("username");
                int count = rs.getInt("count");
                BigDecimal total = rs.getBigDecimal("total");
                result.put(username, new Object[] { count, total });
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return result;
    }

    /**
     * 获取药品销量统计
     * 返回 Map<药品名, [销售数量, 销售额]>
     */
    public Map<String, Object[]> getDrugSalesStats() {
        Map<String, Object[]> result = new HashMap<>();
        String sql = "SELECT d.name, COALESCE(SUM(s.quantity), 0) as qty, COALESCE(SUM(s.total_price), 0) as total " +
                "FROM drug d " +
                "LEFT JOIN sales s ON d.id = s.drug_id " +
                "GROUP BY d.id, d.name " +
                "HAVING qty > 0 " +
                "ORDER BY qty DESC " +
                "LIMIT 20";

        try (Connection conn = DBUtil.getConnection();
                PreparedStatement ps = conn.prepareStatement(sql);
                ResultSet rs = ps.executeQuery()) {

            while (rs.next()) {
                String drugName = rs.getString("name");
                int qty = rs.getInt("qty");
                BigDecimal total = rs.getBigDecimal("total");
                result.put(drugName, new Object[] { qty, total });
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return result;
    }
}
