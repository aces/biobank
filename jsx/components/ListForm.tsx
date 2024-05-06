import React, { useState, useCallback, ReactElement } from 'react';
import { Entity, EntitiesHook } from '../entities';
import { Form } from '../styles/form';
import { Layout } from '../styles/layout';
import { Button } from '../styles/buttons';
import { Input } from './Form';

export const ListForm = <E extends Entity<T>, T extends object>({
  list,
  listItemComponent: ListItemComponent
}: {
  list: EntitiesHook<E, T>,
  listItemComponent: React.ComponentType<{
    id: string,
    entity: E,
    update: EntitiesHook<E, T>['update'],
  }>,
}): ReactElement => {
  const { entities, add, remove, update} = list;
  const [multiplier, setMultiplier] = useState(1);
  const [collapsedStates, setCollapsedStates] = useState(new Map());

  const toggleCollapse = useCallback((key) => {
    setCollapsedStates(prevStates => new Map(prevStates).set(key, !prevStates.get(key)));
  }, []);

  // Duplicate the last subform based on the multiplier
  const duplicate = useCallback(() => {
    if (entities.size > 0) {
      const lastKey = Array.from(entities.keys()).pop(); // Get the last key
      const lastEntity = entities.get(lastKey);
      for (let i = 0; i < multiplier; i++) {
        // Presuming add function creates a new ID automatically and returns it
        const newKey = add(lastEntity.getData());
        setCollapsedStates(prevStates => new Map(prevStates).set(newKey, false));
      }
    }
  }, [entities, multiplier, add]);

  return (
    <>
      {Array.from(entities.entries()).map(([key, entity]) => (
        <ListItem
          id={key}
          collapsed={collapsedStates.get(key)}
          removeSelf={() => remove(key)}
          toggleCollapse={() => toggleCollapse(key)}
        >
          <ListItemComponent id={key} entity={entity} update={list.update}/>
        </ListItem>
      ))}
      <Layout.Row>
        <Button.Add onClick={() => add({})}/>
        <Layout.ActionTitle>New Entry</Layout.ActionTitle>
        <Button.Duplicate onClick={duplicate}/>
        <Layout.ActionTitle>
          <Input
            name='multiplier'
            type='number'
            min={1}
            max={10}
            value={multiplier}
            onUserInput={(name, value) => setMultiplier(Number(value))}
          />
          Copies
        </Layout.ActionTitle>
      </Layout.Row>
    </>
  );
};

const ListItem: React.FC<{
  id: string;
  collapsed: boolean;
  removeSelf: () => void;
  toggleCollapse: () => void;
}> = ({ id, collapsed, removeSelf, toggleCollapse, children }) => {
                                                                             
  return (                                                                   
    <Layout.Grid columns="4fr 1fr">
      <Layout.Item column='1 / 2'>Barcode {id}</Layout.Item>
      <Layout.Item column='2 / 3'>
        <Layout.Row>
          <Form.CollapseButton collapsed={collapsed} onClick={toggleCollapse}/>
          <Button.Remove onClick={removeSelf}/>                              
        </Layout.Row>
      </Layout.Item>
      <Layout.Item column='1 / 3' row='2'>
        <Form.CollapsibleContent collapsed={collapsed}>
          {children}                                                
        </Form.CollapsibleContent>                                                                 
      </Layout.Item>
    </Layout.Grid>
  );    
};

