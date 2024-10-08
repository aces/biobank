<?php declare(strict_types=1);                                                     
                                                                                   
namespace LORIS\biobank;                                                      
                                                                                   
use LORIS\biobank\data\query\QueryBuilder;                                         
use LORIS\biobank\data\query\QueryParams;                                          
use LORIS\biobank\data\query\Condition;                                            
use LORIS\Data\ProvisionerInstance;                                                
use LORIS\biobank\data\Transactions;

/**
 * Class Persistor
 *
 * Handles the persistence of models to the database, including the insertion,
 * updating, and managing of related entities.
 *
 * @package    LORIS
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.j.rabalais@gmail.com>
 * @license    https://www.gnu.org/licenses/gpl-3.0.html GNU General Public
 *             License
 */
class Persistor
{
    use Transactions;                                                              

    /**
     * Constructs the Persistor with a database connection.
     *
     * @param \Database $db The database connection used for persistence
     *                      operations.
     */    
    public function __construct(
        protected \Database $db
    ) {}

    /**
     * Saves a model to the database.
     *
     * This method handles both insertion and updates. It also manages
     * related models by verifying their existence and ensuring proper
     * relationships are maintained in the database.
     *
     * Note: The nested loops and logic for handling related models may
     * introduce performance issues when processing large datasets.
     * Refactoring for efficiency is recommended.
     *
     * Related entities must already exist in the database; otherwise,
     * an exception is thrown, which halts the operation.
     *
     * @param Model $model The model to be saved.
     * @return Model       The saved model with any modifications applied.
     * @throws \Exception  If there are issues with related entities or
     *                     database operations.
     */    
    public function save(Model $model): Model
    {
        $schema = $model->getSchema();
        $table = $schema->getTable()->getName();
        $columns = $schema->getColumns();

        // Extract data from the model based on schema columns
        $data = $this->extractData($model, $columns);

        // Perform insert or update using the database handler
        $this->db->insertOnDuplicateUpdate($table, $data);

        if (!isset($model->id)) {
            $model->id = $this->db->getLastInsertId();
        }

        // Handle related models
        foreach ($schema->getRelations() as $relationName => $relation) {
            $relatedModel = $model->$relationName;

            if ($relatedModel instanceof ModelInterface) {
                $relatedSchema = $relatedModel->getSchema();
                $relatedData = $this->extractData($relatedModel, $relatedSchema->getColumns());

                // Retrieve the label value from related data
                $label = $relatedData[$relatedSchema->getLabel()];

                // Attempt to find existing related record based on label
                $existingId = $this->getIdByLabel($relatedSchema, $label);

                if ($existingId) {
                    // Use existing ID in the main model's data
                    $data[$relation->from->key] = $existingId;
                } else {
                    // Related ID does not exist; throw exception to drop the insert
                    throw new \Exception("Related entity with label '{$label}' does not exist.");
                }
            }

            // Handle many-to-many relations
            if ($relation->many && is_array($relatedModel)) {
                foreach ($relatedModel as $item) {
                    if ($item instanceof ModelInterface) {
                        $relatedSchema = $item->getSchema();
                        $relatedData = $this->extractData($item, $relatedSchema->getColumns());

                        // Retrieve the label value from related data
                        $label = $relatedData[$relatedSchema->getLabel()];

                        // Attempt to find existing related record based on label
                        $existingId = $this->getIdByLabel($relatedSchema, $label);

                        if ($existingId) {
                            // Insert association into the relation table
                            $relationTable = $relation->from->table->getName();
                            $relationData = [
                                $relation->from->key => $data[$schema->getId()],
                                $relation->to->key => $existingId,
                            ];

                            $insertSuccess = $this->db->insertOnDuplicateUpdate($relationTable, $relationData);

                            if (!$insertSuccess) {
                                throw new \Exception("Failed to associate related entity with ID '{$existingId}' in relation table '{$relationTable}'.");
                            }
                        } else {
                            // Related ID does not exist; throw exception to drop the insert
                            throw new \Exception("Related entity with label '{$label}' does not exist.");
                        }
                    }
                }
            }
        }

        return $model;
    }

    /**
     * Extracts data from the model based on schema columns.
     *
     * This method maps the model properties to the database columns defined
     * in the schema. It assumes that the model contains the properties
     * specified in the schema.
     *
     * Note: There is no error handling for missing or unexpected model
     * properties. This can lead to silent failures if a property is
     * missing. Consider adding validation to ensure model consistency.
     *
     * @param ModelInterface $model   The model to extract data from.
     * @param array          $columns The schema columns to extract.
     * @return array                  The extracted data as an associative
     *                                array.
     */    
    protected function extractData(ModelInterface $model, array $columns): array
    {
        $data = [];
        foreach ($columns as $property => $column) {
            if (property_exists($model, $property)) {
                $value = $model->$property;
                // Optionally, handle nulls or defaults here
                $data[$column->getName()] = $value;
            }
        }

        return $data;
    }

    /**
     * Retrieves the ID of a record based on its unique label.
     *
     * This method searches the database for a related entity using its
     * unique label and returns the corresponding ID if found.
     *
     * Note: This method assumes that the label is unique. If multiple
     * records share the same label, this can result in incorrect
     * associations. Ensure that labels are unique, or update this method
     * to handle duplicates more explicitly.
     *
     * @param Schema $schema The schema of the related model.
     * @param string $label  The unique label to search for.
     * @return ?string       The ID of the related entity, or null if not
     *                       found.
     */    
    protected function getIdByLabel(Schema $schema, string $label): ?string
    {
        $condition = new Condition(field: $schema->getLabel(), value: $label);
        $params = (new QueryParams())->where($condition);
        $query = (new QueryBuilder($schema))->build();

        // Use the existing pselectOne function to execute the query
        return $this->db->pselectOne($query->toString(), $query->getParams());
    }
}
