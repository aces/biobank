<?php declare(strict_types=1);

namespace LORIS\biobank\data;

/**
 * Trait Transactions
 *
 * Provides basic transaction management functionality, including
 * begin, commit, and rollback operations. This trait depends on
 * the `$db` property being an instance of a database handler
 * supporting transactions.
 *
 * @package    LORIS
 * @subpackage Biobank
 */
trait Transactions
{
    /**
     * The database instance used for transaction management.
     *
     * @var \Database
     */    
    protected \Database $db;

    /**
     * Begins a database transaction.
     *
     * @return void
     */    
    public function begin() {
        $this->db->beginTransaction();
    }

    /**
     * Commits the current transaction.
     *
     * @return void
     */    
    public function commit() {
        $this->db->commit();
    }

    /**
     * Rolls back the current transaction.
     *
     * @return void
     */    
    public function rollback() {
        $this->db->rollback();
    }
}
