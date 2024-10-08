import { useState } from 'react';
import { useSpecimen, ISpecimen, Specimen, useEntities, Entity, EntitiesHook, SpecimenProvider } from '../entities';
import { useBiobankContext } from '../hooks';
import { ListField, DynamicField, CheckboxField, Button, SpecimenField} from '../forms';

function createInitialState<T extends object>(): Record<keyof T, boolean> {
  return {} as Record<keyof T, boolean>;
}

const EditEntitiesForm = ({
 
}) => {
  const [selected, select] = useState(createInitialState<ISpecimen>());

  const context = useBiobankContext();
  const specimen = useSpecimen(new Specimen({}));
  const specimens = useEntities<Specimen, ISpecimen>(Specimen);

  const handleCheck = (key: keyof ISpecimen, value: boolean) => {
    select(prev => ({ ...prev, [key]: value }));
  };

  const handleSubmit = () => {
    const model = specimen.getData();
    const selectedProperties = Object.entries(selected)
      .filter(([, isSelected]) => isSelected)
      .map(([key]) => key as keyof ISpecimen);
  
    selectedProperties.forEach(property => {
      specimens.setAll(property, model[property]);
    });

    // specimens.validateAll

    // send via API
  };  

  return (
    <>
      <h3>Select Entities to Edit</h3>
      <ListField
        name="specimens"
        label="Select Specimens"
        onAdd={(barcode) => specimens
          .update(barcode, context.specimens.data
                  .find(specimen => specimen.container.barcode == barcode))}
        onRemove={specimens.remove}
        list={specimens.keys().reduce((acc, item) => ({ ...acc, [item]: item }), {})}
        options={context.specimens.data.map(specimen => specimen.container.barcode)}
      />
      <h3>Edit Fields</h3>
      <SpecimenProvider specimen={specimen}>
        {Object.keys(specimen.getData()).map((key: string) => (
          <div key={key}>
            <label>
              <SpecimenField property={key as keyof ISpecimen}/>
              <CheckboxField
                name={key}
                value={selected[key as keyof ISpecimen]}
                onChange={handleCheck}
              />
            </label>
          </div>
        ))}
      </SpecimenProvider>
      <Button label='Submit' onClick={handleSubmit} />
    </>
  );
};

export default EditEntitiesForm;
