<?php declare (strict_types=1);

namespace LORIS\biobank\data\schema;

enum LoadingStrategy: string
{
    case LAZY = 'LAZY';
    case EAGER = 'EAGER';
}

trait LoadingStrategyTrait {
    protected LoadingStrategy $loadingStrategy;

    private function setLoadingStrategy(LoadingStrategy $strategy) {
        $this->loadingStrategy = $strategy;
    }

    public function getLoadingStrategy(): LoadingStrategy {
        return $this->loadingStrategy;
    }

    public function isEager(): bool
    {
        return $this->loadingStrategy === LoadingStrategy::EAGER;
    }

    public function isLazy(): bool
    {
        return $this->loadingStrategy === LoadingStrategy::LAZY;
    }

    public static function eager(): static {
        $instance = new static();
        $instance->setLoadingStrategy(LoadingStrategy::EAGER);
        return $instance;
    }

    public static function lazy(): static {
        $instance = new static();
        $instance->setLoadingStrategy(LoadingStrategy::LAZY);
        return $instance;
    }
}
