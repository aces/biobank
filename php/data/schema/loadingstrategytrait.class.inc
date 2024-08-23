<?php declare (strict_types=1);

namespace LORIS\biobank\data\schema;

enum LoadingStrategy: string
{
    case LAZY = 'LAZY';
    case EAGER = 'EAGER';
    case FORCED = 'FORCED';
}

trait LoadingStrategyTrait {
    //protected LoadingStrategy $strategy;

    public function getLoadingStrategy(): LoadingStrategy {
        return $this->strategy;
    }

    public function isEager(): bool
    {
        return $this->strategy === LoadingStrategy::EAGER;
    }

    public function isForced(): bool
    {
        return $this->strategy === LoadingStrategy::FORCED;
    }

    public function isLazy(): bool
    {
        return $this->strategy === LoadingStrategy::LAZY;
    }

    //public static function eager(): static {
    //    return new static(LoadingStrategy::EAGER);
    //    $instance->setLoadingStrategy(LoadingStrategy::EAGER);
    //}

    //public static function lazy(): static {
    //    return new static(LoadingStrategy::LAZY);
    //}
}
