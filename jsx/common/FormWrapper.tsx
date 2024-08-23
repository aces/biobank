import React, { ReactNode } from 'react';
import { Button } from '../Button';

interface FormWrapperProps {
  onSubmit: (event: React.FormEvent<HTMLFormElement>) => void;
  children: ReactNode;
  submitLabel?: string;
  className?: string;
}

const FormWrapper: React.FC<FormWrapperProps> = ({
  onSubmit,
  children,
  submitLabel = 'Submit',
  className = '',
}) => {
  return (
    <form onSubmit={onSubmit} className={className}>
      {children}
      <div className="form-group mt-3">
        <Button 
          onClick={(e) => {
            e.preventDefault();
            onSubmit(e as unknown as React.FormEvent<HTMLFormElement>);
          }}
          label={submitLabel}
        />
      </div>
    </form>
  );
};

export default FormWrapper;
