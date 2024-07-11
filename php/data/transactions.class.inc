<?php declare(strict_types=1);

namespace LORIS\biobank\data;

trait Transactions {
    protected \Database $db;

    public function begin() {
        $this->db->beginTransaction();
    }

    public function commit() {
        $this->db->commit();
    }

    public function rollback() {
        $this->db->rollback();
    }

    public function delete() {
        $this->db->delete();
    }
}
