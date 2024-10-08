import React, { useState, useEffect, ReactElement, useRef} from 'react';
import { ContainerParentForm, SpecimenField, ContainerField, ProcessForm,
  ListForm } from '../forms';
import Modal from 'Modal';
import { LabelAPI, SpecimenAPI } from '../APIs';
import { mapFormOptions, clone, padBarcode } from '../utils';
import { useBiobankContext } from '../hooks';
import {
  ISpecimen,
  Specimen,
  SpecimenHook,
  SpecimenProvider,
  useSpecimen,
  useContainer,
  useEntities,
  EntitiesHook,
} from '../entities';
import {
  CheckboxField,
  StaticField,
} from './';
import dict from '../i18n/en.json';
declare const loris: any;

type SpecimenFormProps = {
  parent?: SpecimenHook,
  show: boolean,
  onClose: () => void,
  title: string,
};

function SpecimenForm({
  parent,
  show,
  onClose,
  title,
}: SpecimenFormProps): ReactElement {

  const specimen = useSpecimen(new Specimen({}));
  const specimens = useEntities<Specimen, ISpecimen>(Specimen);
  const [printBarcodes, setPrintBarcodes] = useState(false);

  useEffect(() => {
    // specimen.set('parentSpecimens', Object.values(parent).map((item) => item.specimen.id));
    if (parent) {
      specimen.set('candidate', parent.candidate);
      specimen.set('session', parent.session);
      specimen.set('type', parent.type);
      specimen.container.set('center', parent.container.center);
    }

    // if (parent.length > 1) {
    //   specimen.set('quantity', 0)
    // }
  }, [parent]); // Dependency array, effect runs when `parent` changes


  /**
  * Handle the submission of a form
  *
  * @return {Promise}
  */
  const handleSubmit = async () => {

    try {
      if (printBarcodes) {
        await new LabelAPI().create(specimens);
      }
      await new SpecimenAPI().create(specimen);
      // Handle success (e.g., show a success message, navigate to another page, etc.)
    } catch (error) {
      // Handle error (e.g., show an error message, log the error, etc.)
      console.error(error);
    }
  };

  const renderGlobalFields = () => {
    if (parent && specimen.candidate && specimen.session) {
      const parentBarcodes = Object.values(parent).map(
        (item) => item.barcode
      );
      const parentBarcodesString = parentBarcodes.join(', ');
      return (
        <div>
          <StaticField
            label="Parent Specimen(s)"
            value={parentBarcodesString}/>
          <SpecimenField property={'candidate'} isStatic/>
          <SpecimenField property={'session'} isStatic/>
        </div>
      );
    } else {
      return (
        <>
          <SpecimenField property='candidate'/>
          <SpecimenField property='session'/>
        </>
      );
    }
  };


  return (
    <Modal
      title={title}
      show={show}
      onClose={() => onClose()}
      onSubmit={() => new SpecimenAPI().create(specimens)}
      throwWarning={true}
    >
      <SpecimenProvider specimen={specimen}>
        <div className='row'>
          <div className="col-xs-11">
            <StaticField label='Note' value={parent ? dict.noteForAliquots : dict.noteForSpecimens}/>
            {renderGlobalFields()}
            <SpecimenField property={'projects'}/>
            {parent && (
              <>
                <SpecimenField property={'quantity'} />
                <SpecimenField property={'unit'} />
              </>
            )}            
          </div>
        </div>
      </SpecimenProvider>
      <ListForm list={specimens} listItemComponent={SpecimenBarcodeForm}/>
      <br/>
      <div className='form-top'/>
      <ContainerParentForm
        container={specimen.container}
        display={true}
      />
      <div className='form-top'/>
      <CheckboxField
        name='printBarcodes'
        label='Print Barcodes'
        onChange={(name, value) => setPrintBarcodes(!!value)}
        value={printBarcodes}
      />
    </Modal>
  );
}

const SpecimenBarcodeForm: React.FC<{
  id: string,
  entity: Specimen,
  update: EntitiesHook<Specimen, ISpecimen>['update'],
}> = ({
  id,
  entity,
  update,
}) => {

  const specimen = useSpecimen(entity);

  useEffect(() => {
    update(id, specimen.getData());
  }, [specimen.getData()]);

  return (
    <SpecimenProvider specimen={specimen}>
      <ContainerField property={'barcode'}/>
      <SpecimenField property={'type'}/>
      <ContainerField property={'type'}/>
      <ContainerField property={'lotNumber'}/>
      <ContainerField property={'expirationDate'}/>
      <ProcessForm
        edit={true}
        process={specimen.collection}
        processStage='collection'
        type={specimen.type}
      />
    </SpecimenProvider>
  );
}

export default SpecimenForm;
