export enum Operator {
  Equals = '=',
  NotEquals = '!=',
  LessThan = '<',
  GreaterThan = '>',
  LessThanOrEqual = '<=',
  GreaterThanOrEqual = '>=',
  Like = 'like'
}

export interface QueryParam {
  field: string;
  value: string;
  operator?: Operator;
}

export default class QueryBuilder {
  private params: Record<string, string> = {};

  addParam({
    field,
    value,
    operator = Operator.Equals
  }: QueryParam): this {
    const encodedField = encodeURIComponent(field);
    const encodedValue = encodeURIComponent(value);
    this.params[`${encodedField}${this.getOperatorSuffix(operator)}`] = encodedValue;
    return this;
  }

  build(): string {
    return new URLSearchParams(this.params).toString();
  }

  private getOperatorSuffix(operator: Operator): string {
    switch (operator) {
      case Operator.Equals: return '';
      case Operator.NotEquals: return '!=';
      case Operator.LessThan: return '<';
      case Operator.GreaterThan: return '>';
      case Operator.LessThanOrEqual: return '<=';
      case Operator.GreaterThanOrEqual: return '>=';
      case Operator.Like: return '_like';
      default: return '';
    }
  }
}
