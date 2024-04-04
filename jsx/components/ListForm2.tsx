import React, { useState, useCallback, ReactElement } from 'react';

export const ListForm2 = ({ children, list }) => {
  const { entities, add, remove } = list;
  const [multiplier, setMultiplier] = useState(1);
  const [collapsedStates, setCollapsedStates] = useState(new Map());

  // Toggle collapsed state for an entity by its key
  const toggleCollapse = useCallback((key) => {
    setCollapsedStates(prevStates => {
      const newState = new Map(prevStates);
      newState.set(key, !newState.get(key)); // Toggle the state
      return newState;
    });
  }, []);

  // Duplicate the last subform based on the multiplier
  const duplicateLastSubForm = useCallback(() => {
    if (entities.size > 0) {
      const lastKey = Array.from(entities.keys()).pop(); // Get the last key
      const lastSubForm = entities.get(lastKey);
      for (let i = 0; i < multiplier; i++) {
        // Presuming add function creates a new ID automatically and returns it
        const newKey = add({ ...lastSubForm });
        setCollapsedStates(prevStates => new Map(prevStates).set(newKey, false));
      }
    }
  }, [entities, multiplier, add]);

  return (
    <div>
      {Array.from(entities.entries()).map(([key, entity]) => (
        <ListItem
          key={key}
          collapsed={collapsedStates.get(key) || false}
          removeSelf={() => remove(key)}
          toggleCollapse={() => toggleCollapse(key)}
        >
          {React.cloneElement(children, {entity: entity})}
        </ListItem>
      ))}
      <button onClick={() => add({})}>Add Subform</button>
      <input
        type="number"
        value={multiplier}
        onChange={(e) => setMultiplier(Number(e.target.value))}
        placeholder="Multiplier"
      />
      <button onClick={duplicateLastSubForm}>Duplicate Last Subform</button>
    </div>
  );
};

const ListItem = ({ children, collapsed, removeSelf, toggleCollapse }) => {
  return (
    <div className="subform-wrapper">
      <button onClick={toggleCollapse}>
        {collapsed ? "Expand" : "Collapse"}
      </button>
      {!collapsed && children}
      <button onClick={removeSelf}>Remove</button>
    </div>
  );
};
