import { ReactElement } from 'react';
import { Container, Specimen } from '../types';

type LifeCycleProps = {
  specimen: Specimen
  container?: Container;
};

/**
 * Functional component representing the lifecycle of a specimen.
 * Renders different nodes based on the specimen's properties.
 * 
 * @param {LifeCycleProps} props - The props object for the component.
 * @returns {ReactElement} - A React Element representing the component.
 */
function LifeCycle({
  specimen,
  container
}: LifeCycleProps): ReactElement {
  // Conditionally renders the collection node
  const collectionNode = specimen?.collection || container ? (
    <div
      onMouseLeave={(e) => console.log('Mouse leave', e)}
      className="lifecycle-node collection"
    >
      <div className="letter">C</div>
    </div>
  ) : null;

  // Conditionally renders the preparation node
  const preparationNode = specimen?.preparation ? (
    <div className="lifecycle-node preparation">
      <div className="letter">P</div>
    </div>
  ) : null;

  // Conditionally renders the analysis node
  const analysisNode = specimen?.analysis ? (
    <div className="lifecycle-node-container">
      <div className="lifecycle-node">
        <div className="letter">A</div>
      </div>
    </div>
  ) : null;

  // Calculates the number of active nodes and line width
  let nodes = 0;
  if (specimen) {
    nodes = ['collection', 'preparation', 'analysis']
      .reduce((count, key) => specimen[key] ? count + 1 : count, 0);
  }
  const lineWidth = nodes > 1 ? 60 / (nodes - 1) : 0;
  const lineStyle = { width: `${lineWidth}%` };

  // Renders a line connecting nodes, if applicable
  const line = <div className="lifecycle-line" style={lineStyle} />;

  // Main render return
  return (
    <div className="lifecycle">
      <div className="lifecycle-graphic">
        {collectionNode}
        {preparationNode && line}
        {preparationNode}
        {analysisNode && line}
        {analysisNode}
      </div>
    </div>
  );
}

export default LifeCycle;
