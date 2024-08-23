import Modal from 'Modal';
import { PoolField, SpecimenField, StaticField} from '../forms';
import { PoolAPI } from '../APIs';
import { usePool, PoolProvider } from '../entities';
import dict from '../i18n/en.json';

const PoolForm = ({
  onClose,
  show,
}: {onClose: () => void, show: boolean}) => {
  const pool = usePool();

  if (!show) return null;

  return (
    <Modal
      title='Pool Specimens'
      show={show}
      onClose={onClose}
      onSubmit={() => new PoolAPI().create(pool)}
      throwWarning={true}
    >
      <PoolProvider pool={pool}>
        <StaticField label='Pooling Note' value={dict.noteForPools}/>
        <PoolField property={'type'} isStatic/>
        <PoolField property={'candidate'} isStatic/>
        <PoolField property={'session'} isStatic/>
        <PoolField property={'specimens'}/>
        <PoolField property={'label'}/>
        <PoolField property={'quantity'}/>
        <PoolField property={'unit'}/>
        <PoolField property={'date'}/>
        <PoolField property={'time'}/>
      </PoolProvider>
    </Modal>
  );
}

export default PoolForm;
