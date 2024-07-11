<?php declare(strict_types=1);                                                  
                                                                                
namespace LORIS\biobank\factories;                                              
                                                                                
use LORIS\biobank\models\Specimen;                                             
use LORIS\biobank\factories\container\TypeFactory;                              
use LORIS\biobank\factories\ShipmentFactory;                                    
use LORIS\biobank\factories\CenterFactory;                                      
use \DateTime;                                                                  
                                                                                
class SpecimenFactory extends ModelFactory                                     
{                                                                               
    protected static function getModelClass(): string {                         
        return Specimen::class;                                                
    }                                                                           
                                                                                
    protected static function map(string $name, $value)                    
    {                                                                           
        return match ($name) {                                                  
            'id' => $value,                                                     
            'container' => ContainerFactory::create($value),
            'type' => TypeFactory::create($value),
            'quantity' => $value,
            'unit' => UnitFactory::create($value),
            'center' => CenterFactory::create($value),
            'projects' => ProjectFactory::createMany($projects),
            'fTCycle' => $value,
            'parents' => self::createMany($value),
            'candidate' => CandidateFactory::create($value),
            'session' => SessionFactory::create($value),
            'pool' => PoolFactory::create($value),
            'collection' => CollectionFactory::create($value),
            'preparation' => PreparationFactory::create($value),
            'analysis' => AnalysisFactory::create($value),
        };                                                                      
    }                                                                           
}  
