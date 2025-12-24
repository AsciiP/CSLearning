package dao;

import model.Drug;
import util.DBUtil;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class DrugDAO {
    
    public List<Drug> findAll() {
        List<Drug> list = new ArrayList<>();
        String sql = "SELECT id, name, barcode, category_id, supplier_id, price, stock, description, expire_date FROM drug ORDER BY id";
        try (Connection conn = DBUtil.getConnection();
             PreparedStatement ps = conn.prepareStatement(sql);
             ResultSet rs = ps.executeQuery()) {
            while (rs.next()) {
                Drug drug = mapResultSetToDrug(rs);
                list.add(drug);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return list;
    }

    public Drug findById(int id) {
        String sql = "SELECT id, name, barcode, category_id, supplier_id, price, stock, description, expire_date FROM drug WHERE id = ?";
        try (Connection conn = DBUtil.getConnection();
             PreparedStatement ps = conn.prepareStatement(sql)) {
            ps.setInt(1, id);
            ResultSet rs = ps.executeQuery();
            if (rs.next()) {
                return mapResultSetToDrug(rs);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    public Drug findByBarcode(String barcode) {
        String sql = "SELECT id, name, barcode, category_id, supplier_id, price, stock, description, expire_date FROM drug WHERE barcode = ?";
        try (Connection conn = DBUtil.getConnection();
             PreparedStatement ps = conn.prepareStatement(sql)) {
            ps.setString(1, barcode);
            ResultSet rs = ps.executeQuery();
            if (rs.next()) {
                return mapResultSetToDrug(rs);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    public void insert(Drug drug) {
        String sql = "INSERT INTO drug(name, barcode, category_id, supplier_id, price, stock, description, expire_date) VALUES(?, ?, ?, ?, ?, ?, ?, ?)";
        try (Connection conn = DBUtil.getConnection();
             PreparedStatement ps = conn.prepareStatement(sql)) {
            ps.setString(1, drug.getName());
            ps.setString(2, drug.getBarcode());
            ps.setInt(3, drug.getCategoryId());
            ps.setInt(4, drug.getSupplierId());
            ps.setBigDecimal(5, drug.getPrice());
            ps.setInt(6, drug.getStock());
            ps.setString(7, drug.getDescription());
            ps.setDate(8, drug.getExpireDate());
            ps.executeUpdate();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void update(Drug drug) {
        String sql = "UPDATE drug SET name=?, barcode=?, category_id=?, supplier_id=?, price=?, stock=?, description=?, expire_date=? WHERE id=?";
        try (Connection conn = DBUtil.getConnection();
             PreparedStatement ps = conn.prepareStatement(sql)) {
            ps.setString(1, drug.getName());
            ps.setString(2, drug.getBarcode());
            ps.setInt(3, drug.getCategoryId());
            ps.setInt(4, drug.getSupplierId());
            ps.setBigDecimal(5, drug.getPrice());
            ps.setInt(6, drug.getStock());
            ps.setString(7, drug.getDescription());
            ps.setDate(8, drug.getExpireDate());
            ps.setInt(9, drug.getId());
            ps.executeUpdate();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void delete(int id) {
        String sql = "DELETE FROM drug WHERE id = ?";
        try (Connection conn = DBUtil.getConnection();
             PreparedStatement ps = conn.prepareStatement(sql)) {
            ps.setInt(1, id);
            ps.executeUpdate();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private Drug mapResultSetToDrug(ResultSet rs) throws SQLException {
        Drug drug = new Drug();
        drug.setId(rs.getInt("id"));
        drug.setName(rs.getString("name"));
        drug.setBarcode(rs.getString("barcode"));
        drug.setCategoryId(rs.getInt("category_id"));
        drug.setSupplierId(rs.getInt("supplier_id"));
        drug.setPrice(rs.getBigDecimal("price"));
        drug.setStock(rs.getInt("stock"));
        drug.setDescription(rs.getString("description"));
        drug.setExpireDate(rs.getDate("expire_date"));
        return drug;
    }
}
