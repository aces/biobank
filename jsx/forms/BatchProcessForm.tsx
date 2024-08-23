import React, { useState, ReactElement, useMemo } from 'react';
import {VerticalTabs, TabPane} from 'Tabs';
import Modal from 'Modal';
import { StaticField, ListField, ProcessForm, PoolField, SpecimenField } from '../forms';
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
  show: boolean,
}> = ({
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

  return (
    <Modal
      title='Process Specimens'
      show={show}
      onSubmit={() => Promise.resolve()}
      onClose={clear}
      throwWarning={true}
    >
      <StaticField label='Processing Note' value={dict.batchProcessNote}/>
      <SpecimenField property={'type'} isStatic/>
      <ListField
        name={'specimens'}
        label={'Barcode'}
        onAdd={specimens.update}
        onRemove={specimens.remove}
        list={specimens.toData()}
        options={context.specimensi.data}
        format={(specimen) => specimen.container.barcode}
      />
      {editForms}
    </Modal>
  );
}

export default BatchProcessForm;
