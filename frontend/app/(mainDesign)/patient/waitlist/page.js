"use client";

import { useState, useEffect } from "react";
import { useRouter } from 'next/navigation';
import Link from "next/link";

export default function PatientWaitlistPage() {
    const [waitlistItems, setWaitlistItems] = useState([]);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState("");
    const [cancelingId, setCancelingId] = useState(null);
    const [cancelSuccess, setCancelSuccess] = useState("");
    const router = useRouter();
    const [userId, setUserId] = useState(null);

    useEffect(() => {
        // Проверка аутентификации
        const isLoggedIn = localStorage.getItem('isLoggedIn');
        const userData = JSON.parse(localStorage.getItem('medSchedulerUser') || '{}');
        
        if (!isLoggedIn || !userData.userId || userData.userType !== 'patient') {
            router.push('/login');
            return;
        }

        setUserId(userData.userId);
        fetchWaitlist(userData.userId);
    }, [router]);

    const fetchWaitlist = async (patientId) => {
        try {
            // Запрос на получение данных листа ожидания
            const response = await fetch(`https://api.medscheduler.ru/get_patient_waitlist?patient_id=${patientId}`);
            const data = await response.json();

            if (response.ok && data.success) {
                setWaitlistItems(data.waitlist || []);
            } else {
                setError(data.error || "Ошибка при получении данных листа ожидания");
            }
        } catch (err) {
            console.error("Error fetching waitlist:", err);
            setError("Не удалось подключиться к серверу");
        } finally {
            setLoading(false);
        }
    };

    const handleCancelWaitlist = async (waitlistId) => {
        setCancelingId(waitlistId);
        setError("");
        setCancelSuccess("");

        try {
            // API запрос на отмену записи в листе ожидания
            const response = await fetch('https://api.medscheduler.ru/cancel_appointment_from_waitlist', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ waitlist_id: waitlistId, patient_id: userId }),
            });

            const data = await response.json();

            if (response.ok && data.success) {
                setCancelSuccess("Запись из листа ожидания успешно отменена");
                
                // Обновляем список записей (удаляем отмененную)
                setWaitlistItems(waitlistItems.filter(item => item.waitlist_id !== waitlistId));
                
                // Сбрасываем сообщение об успехе через 3 секунды
                setTimeout(() => {
                    setCancelSuccess("");
                }, 3000);
            } else {
                setError(data.error || "Ошибка при отмене записи из листа ожидания");
            }
        } catch (err) {
            console.error("Error canceling waitlist:", err);
            setError("Не удалось подключиться к серверу");
        } finally {
            setCancelingId(null);
        }
    };

    // Форматирование даты
    const formatDate = (dateStr) => {
        const date = new Date(dateStr);
        return date.toLocaleDateString('ru-RU', { 
            day: '2-digit', 
            month: '2-digit',
            year: 'numeric'
        });
    };

    if (loading) {
        return (
            <div className="min-h-[calc(80vh-theme(space.36))] flex items-center justify-center">
                <div className="text-xl text-gray-600">Загрузка данных листа ожидания...</div>
            </div>
        );
    }

    return (
        <div className="max-w-4xl mx-auto py-8 px-4">
            <h1 className="text-3xl font-bold text-main2 mb-8">Мои записи в листе ожидания</h1>
            
            {error && (
                <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-4">
                    {error}
                </div>
            )}
            
            {cancelSuccess && (
                <div className="bg-green-100 border border-green-400 text-green-700 px-4 py-3 rounded mb-4">
                    {cancelSuccess}
                </div>
            )}
            
            {waitlistItems.length === 0 ? (
                <div className="bg-white shadow-md rounded-lg p-6 text-center">
                    <p className="text-lg text-gray-600 mb-4">У вас нет активных записей в листе ожидания</p>
                    <Link href="/search" className="px-4 py-2 bg-main text-white rounded hover:bg-main2 transition-colors">
                        Найти врача
                    </Link>
                </div>
            ) : (
                <div className="bg-white shadow-md rounded-lg overflow-hidden">
                    <table className="min-w-full divide-y divide-gray-200">
                        <thead className="bg-gray-50">
                            <tr>
                                <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                    Врач
                                </th>
                                <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                    Специальность
                                </th>
                                <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                    Клиника
                                </th>
                                <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                    Дата добавления
                                </th>
                                <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                    Позиция
                                </th>
                                <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                    Действия
                                </th>
                            </tr>
                        </thead>
                        <tbody className="bg-white divide-y divide-gray-200">
                            {waitlistItems.map((item) => (
                                <tr key={item.waitlist_id}>
                                    <td className="px-6 py-4 whitespace-nowrap">
                                        <Link href={`/doctor/${item.doctor_id}`} className="text-main hover:text-main2">
                                            <div className="text-sm font-medium">{item.doctor_name}</div>
                                        </Link>
                                    </td>
                                    <td className="px-6 py-4 whitespace-nowrap">
                                        <div className="text-sm text-gray-500">{item.specialty}</div>
                                    </td>
                                    <td className="px-6 py-4 whitespace-nowrap">
                                        <div className="text-sm text-gray-500">{item.hospital_name}</div>
                                    </td>
                                    <td className="px-6 py-4 whitespace-nowrap">
                                        <div className="text-sm text-gray-500">{formatDate(item.added_date)}</div>
                                    </td>
                                    <td className="px-6 py-4 whitespace-nowrap">
                                        <div className="text-sm font-medium">
                                            {item.position === 1 ? (
                                                <span className="text-green-600">{item.position} (Следующий в очереди)</span>
                                            ) : (
                                                <span>{item.position}</span>
                                            )}
                                        </div>
                                    </td>
                                    <td className="px-6 py-4 whitespace-nowrap text-sm font-medium">
                                        <button
                                            onClick={() => handleCancelWaitlist(item.waitlist_id)}
                                            disabled={cancelingId === item.waitlist_id}
                                            className="text-red-600 hover:text-red-900 disabled:opacity-50"
                                        >
                                            {cancelingId === item.waitlist_id ? "Отмена..." : "Отменить"}
                                        </button>
                                    </td>
                                </tr>
                            ))}
                        </tbody>
                    </table>
                </div>
            )}

            <div className="mt-8">
                <p className="text-sm text-gray-600 mb-4">
                    <strong>Что такое лист ожидания?</strong> Если у выбранного врача нет свободных слотов для записи,
                    вы можете добавить себя в лист ожидания. Когда появится свободное время для приема (например,
                    если кто-то отменит запись), вам будет предложено записаться на освободившееся время в порядке очереди.
                </p>
            </div>

            <div className="flex justify-between mt-6">
                <Link href="/patient/appointments" className="text-main hover:text-main2 transition-colors">
                    ← Мои записи к врачам
                </Link>
                <Link href="/search" className="text-main hover:text-main2 transition-colors">
                    Поиск врачей →
                </Link>
            </div>
        </div>
    );
}
