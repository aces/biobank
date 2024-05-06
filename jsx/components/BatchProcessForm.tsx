import React, { useState, ReactElement, useMemo } from 'react';
import {VerticalTabs, TabPane} from 'Tabs';
import Modal from 'Modal';
import Form from 'Form';
const { StaticElement, FormElement, SearchableDropdown } = Form;
import { ProcessForm, PoolField, SpecimenField } from '../components';
import { useBiobankContext, useEditable } from '../hooks';
import { IContainer, IProcess, Specimen, ISpecimen, useProcess, usePool,
  useEntities, EntitiesHook,
} from '../entities';
import Swal, { SweetAlertOptions, SweetAlertResult } from 'sweetalert2';
import dict from '../i18n/en.json'

/** XXX
 *  Remember that the specimen type should determine the current type
 *  When a pool is selected, it should generate specimen list items based on the
 *  pool
 *
 *
 */
const BatchProcessForm: React.FC<{
  onClose: () => void,
  show: boolean,
}> = ({
  onClose,
  show,
}) => {

  const context = useBiobankContext();
  const preparation = useProcess();
  const pool = usePool();
  const specimens = useEntities(Specimen);
  const { editable, edit, clear } = useEditable();
  const type = !specimens.isEmpty() ? specimens.toArray()[0].getData().type
    : null;

  /**
   * Validates a container in the list.
   * 
   * @param {string} container - The ID of the container to validate.
   * @returns {boolean} - Returns true if validation is successful, false otherwise.
   */
  const validateList = (
    list: { specimen: ISpecimen, container: IContainer }[]
  ): Promise<typeof list> => {
    return new Promise((resolve, reject) => {
      const barcodes = Object.values(list)
        .filter(item => !!item.specimen.preparation)
        .map(item => item.container.barcode);
  
      if (barcodes.length > 0) {
        Swal.fire({
          title: 'Warning!',
          html: `Preparation for specimen(s) <b>${barcodes.join(', ')}</b> ` +
                `already exists. By completing this form, the previous ` +
                `preparation will be overwritten.`,
          icon: 'warning' as any,
          showCancelButton: true,
          confirmButtonText: 'Proceed'
        } as SweetAlertOptions).then((result: SweetAlertResult) => {
          // 'result as any' is a temporary bypass because typescript does not
          // recognized isConfirmed as a property of 'result'.
          if ((result as any).isConfirmed) {
            resolve(list);
          } else {
            reject();
          }
        });
      } else {
        resolve(list);
      }
    });
  };

  // TODO: This should likely be filtered so that only pools that match the
  // proper criteria are left in the list.
  const glyphStyle = {
    color: '#DDDDDD',
    marginLeft: 10,
    cursor: 'pointer',
  };

  let barcodeList = [];
  specimens.entities.forEach((specimen, key) => {
    const handleRemoveItem = () => specimens.remove(key);
    barcodeList.push(
      <div key={key} className='preparation-item'>
        <div>{specimen.getData().container.barcode}</div>
        <div
          className='glyphicon glyphicon-remove'
          style={glyphStyle}
          onClick={handleRemoveItem}
        />
      </div>
    );
  });

  const editForms = !specimens.isEmpty() && (
    <div className='form-top'>
      <VerticalTabs
        tabs={[{id: 'preparation', label: 'Preparation'}]}
        onTabChange={(id) => edit(id)}
        updateURL={false}
      >
        <TabPane TabId='preparation'>
          {editable.preparation ? (
            <ProcessForm
              edit={true}
              process={preparation}
              processStage='preparation'
              type={type}
            />
          ) : null}
        </TabPane>
      </VerticalTabs>
    </div>
  );

  const handleClose = () => {
    //setState(initialState);
    onClose();
  };

  const handleSubmit = () => {
    // const prepList = Array.toArray(specimens.entities).map((specimen) => {
    //   specimen.set('preparation', preparation);
    //   specimen.preparation.center = specimen.container.center;
    //   return specimen;
    // });

    // TODO: execute update of specimens here!
  };

 // const [barcode, setBarcode] = useState<string>('');
 // const { specimens: contextSpecimens } = useBiobankContext();
 // 
 // const validateListItem = (barcode: string): boolean => {
 //   // const specimen = context.specimens[barcode] as ISpecimen;
 // 
 //   // if (!specimens.isEmpty() && specimen.type !== type) {
 //   //   Swal.fire('Oops!', 'Specimens must be of the same Type', 'warning');
 //   //   return false;
 //   // }
 // 
 //   return true;
 // };
 
 // const handleInput = async (name: string, value: string) => {
 //   setBarcode(barcode);
 //   if (validateListItem(barcode)) {
 //     specimens.add(contextSpecimens.barcode);
 //     setBarcode(''); // Reset barcode input on successful add
 //   }
 // };

  return (
    <Modal
      title='Process Specimens'
      show={show}
      onClose={handleClose}
      onSubmit={handleSubmit}
      throwWarning={true}
    >
      <StaticElement label='Processing Note' text={dict.batchProcessNote}/>
      <SpecimenField property={'type'} isStatic={true}/>
      <Form.InputList label={'Barcode'} items={specimens} options={context.specimens}/>
      {editForms}
    </Modal>
  );
}

export default BatchProcessForm;
